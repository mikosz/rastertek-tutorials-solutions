#ifndef _SYSTEM_DEBUG_HPP_
#define _SYSTEM_DEBUG_HPP_

namespace tutorials {
namespace system {

#if defined(NDEBUG)
const bool DEBUG = false;
#else
const bool DEBUG = true;
#endif /* NDEBUG */

} // namespace system
} // namespace tutorials

#endif /* _SYSTEM_DEBUG_HPP_ */
