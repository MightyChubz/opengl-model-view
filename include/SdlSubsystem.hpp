#ifndef SUBSYSTEM_HPP
#define SUBSYSTEM_HPP

class SdlSubsystem
{
  public:
    SdlSubsystem();
    SdlSubsystem(const SdlSubsystem &)            = delete;
    SdlSubsystem(SdlSubsystem &&)                 = delete;
    SdlSubsystem &operator=(const SdlSubsystem &) = delete;
    SdlSubsystem &operator=(SdlSubsystem &&)      = delete;
    ~SdlSubsystem();
};

#endif
