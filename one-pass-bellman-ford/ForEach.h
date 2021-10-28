#ifndef FOREACH_H
#define FOREACH_H

// The following macros are derived from work done by William Swanson and released to the public domain in 2012.
// A description can be found as response 55 to the stackoverflow question posted at: https://stackoverflow.com/questions/6707148/foreach-macro-on-macros-arguments/13459454#13459454
// He posted the source at:https://github.com/swansontec/map-macro
//
// We use his code, acknowledge his work, and thank him for releasing it to the public domain.

#define _FOR_EACH_END_RECURSION(...)

#define _FOR_EACH_GET_END2() 0, _FOR_EACH_END_RECURSION
#define _FOR_EACH_GET_END1(...) _FOR_EACH_GET_END2
#define _FOR_EACH_GET_END(...) _FOR_EACH_GET_END1

#define _FOR_EACH_BLOCK_RECURSION

#define _FOR_EACH_EVAL0(...) __VA_ARGS__
#define _FOR_EACH_EVAL1(...) _FOR_EACH_EVAL0(_FOR_EACH_EVAL0(_FOR_EACH_EVAL0(_FOR_EACH_EVAL0(__VA_ARGS__))))
#define _FOR_EACH_EVAL2(...) _FOR_EACH_EVAL1(_FOR_EACH_EVAL1(_FOR_EACH_EVAL1(_FOR_EACH_EVAL1(__VA_ARGS__))))
#define _FOR_EACH_EVAL3(...) _FOR_EACH_EVAL2(_FOR_EACH_EVAL2(_FOR_EACH_EVAL2(_FOR_EACH_EVAL2(__VA_ARGS__))))
#define _FOR_EACH_EVAL4(...) _FOR_EACH_EVAL3(_FOR_EACH_EVAL3(_FOR_EACH_EVAL3(_FOR_EACH_EVAL3(__VA_ARGS__))))
#define _FOR_EACH_EVAL(...)  _FOR_EACH_EVAL4(_FOR_EACH_EVAL4(_FOR_EACH_EVAL4(_FOR_EACH_EVAL4(__VA_ARGS__))))

#define _FOR_EACH_NEXT0(item, next, ...) next _FOR_EACH_BLOCK_RECURSION
#define _FOR_EACH_NEXT1(item, next) _FOR_EACH_NEXT0(item, next, 0)
#define _FOR_EACH_NEXT(item, next)  _FOR_EACH_NEXT1(_FOR_EACH_GET_END item, next)

#define _FOR_EACH0(functionName, listEntry, listNextEntry, ...) functionName(listEntry) _FOR_EACH_NEXT(listNextEntry, _FOR_EACH1)(functionName, listNextEntry, __VA_ARGS__)
#define _FOR_EACH1(functionName, listEntry, listNextEntry, ...) functionName(listEntry) _FOR_EACH_NEXT(listNextEntry, _FOR_EACH0)(functionName, listNextEntry, __VA_ARGS__)
#define _FOR_EACH(functionName, ...) _FOR_EACH_EVAL(_FOR_EACH1(functionName, ## __VA_ARGS__, ()()(), 0))

#endif
