#pragma once
#ifndef CONST_H
#define CONST_H

#define CRLF "\r\n"
#define RNRN "\r\n\r\n"
#define CONTENT_LENGTH "Content-Length"
#define CONTENT_TYPE "Content-Type"
#define TRANSFER_ENCODING "Transfer-Encoding"
#define SET_COOKIE "Set-Cookie"
#define COOKIE "Cookie"
#define MULTIPART_FORM_DATA "multipart/form-data"

#ifndef __APPLE__
#define __APPLE__ 0
#endif

#define BUFFER_SIZE 100000
#define FT 42
#define CRLF_LEN 2

#define FD_ERROR -1
#define FD_LISTING -2
#define FD_ZERO_ -3
#define SYSTEMCALL_ERROR -1

#define SESSION_EXPIRED_TIME 60

#endif
