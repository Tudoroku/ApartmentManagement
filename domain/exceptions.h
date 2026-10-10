#ifndef LAB6_7_EXCEPTIONS_H
#define LAB6_7_EXCEPTIONS_H

#include <exception>
#include <string>
#include <utility>

class AppException : public std::exception
{
private:
    std::string mesaj;

public:
    explicit AppException(std::string mesaj);
    [[nodiscard]] const char* what() const noexcept override;
};

class ValidationException : public AppException
{
public:
    explicit ValidationException(std::string mesaj);
};

class RepoException : public AppException
{
public:
    explicit RepoException(std::string mesaj);
};

class ServiceException : public AppException
{
public:
    explicit ServiceException(std::string mesaj);
};

#endif // LAB6_7_EXCEPTIONS_H
