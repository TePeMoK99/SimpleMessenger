#pragma once

enum MessageTypes
{
    USER_JOIN = 1,
    USER_LEFT,
    USERS_LIST,
    USERS_LIST_REQUEST,
    PUBLIC_MESSAGE,
    PRIVATE_MESSAGE,
    AUTH_REQUEST,
    AUTH_SUCCESS,
    AUTH_FAIL
};
