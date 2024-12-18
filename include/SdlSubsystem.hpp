#ifndef SUBSYSTEM_HPP
#define SUBSYSTEM_HPP

class SdlSubsystem
{
    bool m_successfulInit{true};

  public:
    SdlSubsystem();
    SdlSubsystem(const SdlSubsystem &)            = delete;
    SdlSubsystem(SdlSubsystem &&)                 = delete;
    SdlSubsystem &operator=(const SdlSubsystem &) = delete;
    SdlSubsystem &operator=(SdlSubsystem &&)      = delete;
    ~SdlSubsystem();

    [[nodiscard]] constexpr bool IsInitialized() const
    {
        return m_successfulInit;
    }
};

#endif
