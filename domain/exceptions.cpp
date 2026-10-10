#include "exceptions.h"

AppException::AppException(std::string mesaj) : mesaj(std::move(mesaj)) {}

const char* AppException::what() const noexcept
{
    return mesaj.c_str();
}

ValidationException::ValidationException(std::string mesaj) : AppException(std::move(mesaj)) {}

RepoException::RepoException(std::string mesaj) : AppException(std::move(mesaj)) {}

ServiceException::ServiceException(std::string mesaj) : AppException(std::move(mesaj)) {}
