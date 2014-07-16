#ifndef __LLL_LOGGER_H__
#define __LLL_LOGGER_H__
/***********************************************************************
 * Low Latency Library: Logger
 *
 * Released under the MIT license. The LICENSE file should be included
 * in the top level of the source tree
 ***********************************************************************/
#include <new>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <type_traits>

namespace lll {
namespace logging {

class logitem {
protected:
    void *data;
    void (*logit)(void *self, char *(*formatter)(const char *format, ...));
public:
    void format(char *(*formatter)(const char *format, ...)) {
	logit(this, formatter);
    }
};

namespace internal {

template <typename... T> 
class Inner : public logitem {
public:
    Inner(char *, size_t) {}

    template <typename... Arguments>
    inline void printinner(char *(*formatter)(const char *format, ...), const char *format, Arguments... as)
    {
        formatter(format, as...);
    }
};

template <typename Head, typename... Rest>
class Inner <Head, Rest...> : public Inner<Rest...> { 
protected:
    typename std::remove_reference<Head>::type val_;
public:
    Inner(char *&strings, size_t &stringsize, Head val, Rest... rest) : Inner<Rest...>(strings, stringsize, rest...), val_(val) {}

    template <typename... Arguments>
    inline void printinner(char *(*formatter)(const char *format, ...), const char *format, Arguments... as) {
        Inner<Rest...>::printinner(formatter, format, as..., val_);
    }
};

template <typename... Rest>
class Inner <const char *, Rest...> : public Inner<Rest...> { 
    constexpr const char *nospace() const { return "INSUFFICIENT_LOG_BUFFER_SPACE"; }
    constexpr const char *nosize() const { return "INVALID_SIZE_ON_LOG"; }
protected:
    char *val_;
public:
    Inner(char *&strings, size_t &stringsize, const char * val, Rest... rest) : Inner<Rest...>(strings, stringsize, rest...) {
        if (!stringsize) {
            val_ = (char *)nosize();
            return;
        }

        val_ = strings;
        strings = (char *)memccpy(strings, val, 0x00, stringsize);

        if (!strings) {
            val_ = (char *)nospace();
            stringsize = 0;
        } else {
            stringsize -= (strings - val_);
        }
    }

    template <typename... Arguments>
    inline void printinner(char *(*formatter)(const char *format, ...), const char *format, Arguments... as) {
        Inner<Rest...>::printinner(formatter, format, as..., val_);
    }
};

template <typename... Arguments>
class Outer : public Inner<Arguments...> {
protected:
    const char *format_;
    
public:
    Outer(const char *format, 
          char *strings,
          size_t stringsize,
          Arguments... as) : Inner<Arguments...>(strings, stringsize, as...), format_(format) {
	logitem::data = this;
	logitem::logit = print;
    }

    static void print(void *self, char *(*formatter)(const char *format, ...)) {
        auto self_ = static_cast<Outer<Arguments...> *>(self);
        self_->Inner<Arguments...>::printinner(formatter, self_->format_);
    }
};

static inline void log_format_check_(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
static inline void log_format_check_(const char *, ...) {
};

template <typename... Arguments> 
static logitem *log_(void *vbuffer, size_t buffer_size, const char *format, Arguments... as)  {
    char *buffer = (char *)vbuffer;

    constexpr size_t objsize = sizeof(Outer<Arguments...>);

    if (objsize > buffer_size)
        return nullptr;

    auto logobj = new (buffer) Outer<Arguments...>(format, 
                                                   buffer + objsize,
                                                   buffer_size - objsize,
                                                   as...);
    return logobj;
}

}

#define lll_log(b,s, format, ...) (::lll::logging::internal::log_format_check_(format, __VA_ARGS__), \
				   ::lll::logging::internal::log_(b,s,format, __VA_ARGS__))

}
}
#endif
