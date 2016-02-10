#ifndef __LLL_LOGGER_H__
#define __LLL_LOGGER_H__
/***********************************************************************
 * Low Latency Library: Logger
 *
 * A logger that enqueues log data without formatting, with a separate
 * thread that formats.
 * Released under the MIT license. The LICENSE file should be included
 * in the top level of the source tree
 ***********************************************************************/
#include <new>
#include <cstdarg>
#include <string>
#include <cstdio>
#include <type_traits>

namespace lll {
namespace logging {

/***********************************************************************
 * Everything in the "internal" namespace is used internally to thea
 * logging code 
 ***********************************************************************/
namespace internal {

class logitem {
protected:
    void *_data;
    int (*_formatfunc)(const logitem *self, char * str, size_t size);

public:

    int format(char * str, size_t size) const
    {
	return _formatfunc(this, str, size);
    }
};

/***********************************************************************
 * Terminal class template
 ***********************************************************************/
template <typename... T> 
class log_inner_ : public logitem {
public:
    log_inner_(char *, size_t) {}

    template <typename... Arguments>
    inline int format(char * str, size_t size,
		      const char *format, Arguments... as) const
    {
	return std::snprintf(str, size, format, as...);
    }
};

/***********************************************************************
 * Recursive class template handing non-string items
 ***********************************************************************/
template <typename Head, typename... Rest>
class log_inner_ <Head, Rest...> : public log_inner_<Rest...> { 
protected:
    typename std::remove_reference<Head>::type _val;

public:

    log_inner_(char *&strings, size_t &stringsize, Head val, Rest... rest) :
	log_inner_<Rest...>(strings, stringsize, rest...), _val(val) {}

    template <typename... Arguments>
    inline int format(char * str, size_t size,
		      const char *format, Arguments... as) const
    {
        return log_inner_<Rest...>::format(str, size, format, as..., _val);
    }
};

/***********************************************************************
 * Recursive class template handing const char * items
 ***********************************************************************/
template <typename... Rest>
class log_inner_ <const char *, Rest...> : public log_inner_<Rest...> { 

    constexpr const char *nospace() const
    {
	return "INSUFFICIENT_LOG_BUFFER_SPACE";
    }

    constexpr const char *nosize() const
    {
	return "INVALID_SIZE_ON_LOG";
    }
    
protected:

    char *_val;

public:

    log_inner_(char *&strings, size_t &stringsize,
	       const char * val, Rest... rest) :
	log_inner_<Rest...>(strings, stringsize, rest...)
    {
        if (!stringsize) {
            _val = (char *)nosize();
            return;
        }

        _val = strings;
        strings = (char *)memccpy(strings, val, 0x00, stringsize);

        if (!strings) {
            _val = (char *)nospace();
            stringsize = 0;
        } else {
            stringsize -= (strings - _val);
        }
    }

    template <typename... Arguments>
    inline int format(char * str, size_t size,
		      const char *format, Arguments... as) const
    { 
        return log_inner_<Rest...>::format(str, size, format, as..., _val);
    }
};

/***********************************************************************
 * Outer template class...store the format string and call the inner
 * class
 ***********************************************************************/
template <typename... Arguments>
class log_outer_ : public log_inner_<Arguments...> {
protected:

    const char *_formatstr;
    
public:

    log_outer_(const char *formatstr, 
          char *strings,
          size_t stringsize,
          Arguments... as) :
	log_inner_<Arguments...>(strings, stringsize, as...),
	_formatstr(formatstr)
    {
	logitem::_data = this;
	logitem::_formatfunc = format;
    }

    static int format(const logitem *self, char * str, size_t size)
    {
        auto self_ = static_cast<const log_outer_<Arguments...> *>(self);
        return self_->log_inner_<Arguments...>::format(str, size,
						       self_->_formatstr);
    }
};

/***********************************************************************
 * gcc doesn't do "printf format" checking properly for variadic 
 * templates.  So we have to use a preprocessor macro, and first call
 * this dummy check function, then call our variadic function. The sole
 * purpose of this is to throw an error if the arguments don't match
 * the printf formatting
 ***********************************************************************/
static inline void log_format_check_(const char *, ...)
    __attribute__ ((format (printf, 1, 2)));

static inline void log_format_check_(const char *, ...)
{
};

/***********************************************************************
 * The static function that actually does the logging
 ***********************************************************************/
template <typename... Arguments> 
static inline size_t log_(void *vbuffer, size_t buffer_size,
			  const char *formatstr, Arguments... as)
{
    char *buffer = (char *)vbuffer;

    constexpr size_t objsize = sizeof(log_outer_<Arguments...>);

    if (objsize > buffer_size)
        return 0;

    new (buffer) log_outer_<Arguments...>(formatstr, 
					  buffer + objsize,
					  buffer_size - objsize,
					  as...);
    return objsize;
}

} // namespace internal

#define lll_log(b,s, format, ...)					\
    (::lll::logging::internal::log_format_check_(format, __VA_ARGS__),	\
     ::lll::logging::internal::log_(b,s,format, __VA_ARGS__))

static inline int logformat(void *item, char * str, size_t size)
{
    return static_cast<const internal::logitem *>(item)->format(str, size);
}

} // namespace logging
} // namespace lll

#endif
