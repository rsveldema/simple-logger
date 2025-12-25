#pragma once

#include <format>
#include <string>

/**
 * @file Error.hpp
 * @brief Defines error codes and conversion functions for network operations.
 */

namespace error
{

enum class Error
{
    OK,
    RANGE,
    MMAP_FAILED,
    FAILED_TO_CREATE_SOCKET,
    FAILED_TO_OPEN_PCM,
    ALSA_FAILURE,
    NO_ALSA_CAPTURE,
    NO_ALSA_PLAYBACK,
    HOSTNAME_RESOLVE_FAILED,
    BAD_PPROTOCOL,
    INVALID_SDP,
    UNKNOWN,
    NOT_READY
};

Error errno_to_error(int err);
} // namespace error


template <> struct std::formatter<error::Error>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const error::Error& c, std::format_context& ctx) const
    {
        switch (c) {
        case error::Error::OK:
            return std::format_to(ctx.out(), "OK");
        case error::Error::NOT_READY:
            return std::format_to(ctx.out(), "NOT_READY");
        case error::Error::RANGE:
            return std::format_to(ctx.out(), "RANGE");
        case error::Error::MMAP_FAILED:
            return std::format_to(ctx.out(), "MMAP_FAILED");
        case error::Error::FAILED_TO_CREATE_SOCKET:
            return std::format_to(ctx.out(), "FAILED_TO_CREATE_SOCKET");
        case error::Error::FAILED_TO_OPEN_PCM:
            return std::format_to(ctx.out(), "FAILED_TO_OPEN_PCM");
        case error::Error::ALSA_FAILURE:
            return std::format_to(ctx.out(), "ALSA_FAILURE");
        case error::Error::NO_ALSA_CAPTURE:
            return std::format_to(ctx.out(), "NO_ALSA_CAPTURE");
        case error::Error::NO_ALSA_PLAYBACK:
            return std::format_to(ctx.out(), "NO_ALSA_PLAYBACK");
        case error::Error::HOSTNAME_RESOLVE_FAILED:
            return std::format_to(ctx.out(), "HOSTNAME_RESOLVE_FAILED");
        case error::Error::UNKNOWN:
            return std::format_to(ctx.out(), "UNKNOWN");
        case error::Error::BAD_PPROTOCOL:
            return std::format_to(ctx.out(), "BAD_PPROTOCOL");
        case error::Error::INVALID_SDP:
            return std::format_to(ctx.out(), "INVALID_SDP");
        }
        return std::format_to(ctx.out(), "UNKNOWN_ERROR_CODE");
    }
};
