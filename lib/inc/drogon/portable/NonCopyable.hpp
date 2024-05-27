#ifndef DROGON_NONCOPYABLE_HPP
#define DROGON_NONCOPYABLE_HPP

namespace drogon
{

class NonCopyable
{
  protected:
    NonCopyable()
    {
    }
    ~NonCopyable() = default;

  public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
    // some uncopyable classes maybe support move constructor....
    NonCopyable(NonCopyable &&) noexcept(true) = default;
    NonCopyable &operator=(NonCopyable &&) noexcept(true) = default;
};

}  // namespace drogon

#endif  // DROGON_NONCOPYABLE_HPP
