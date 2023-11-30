/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 00:32:31 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/23 00:37:16 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Types.hpp"

Types::Types( void )
{
    ContentType.push_back("application/java-archive");
    ContentType.push_back("application/EDI-X12");
    ContentType.push_back("application/EDIFACT");
    ContentType.push_back("application/javascript");
    ContentType.push_back("application/octet-stream");
    ContentType.push_back("application/ogg");
    ContentType.push_back("application/pdf");
    ContentType.push_back("application/xhtml+xml");
    ContentType.push_back("application/x-shockwave-flash");
    ContentType.push_back("application/json");
    ContentType.push_back("application/ld+json");
    ContentType.push_back("application/xml");
    ContentType.push_back("application/zip");
    ContentType.push_back("application/x-www-form-urlencoded");
    ContentType.push_back("audio/mpeg");
    ContentType.push_back("audio/x-ms-wma");
    ContentType.push_back("audio/vnd.rn-realaudio");
    ContentType.push_back("audio/x-wav");
    ContentType.push_back("image/gif");
    ContentType.push_back("image/jpeg");
    ContentType.push_back("image/png");
    ContentType.push_back("image/tiff");
    ContentType.push_back("image/vnd.microsoft.icon");
    ContentType.push_back("image/x-icon");
    ContentType.push_back("image/vnd.djvu");
    ContentType.push_back("image/svg+xml");
    ContentType.push_back("multipart/mixed");
    ContentType.push_back("multipart/alternative");
    ContentType.push_back("multipart/related");
    ContentType.push_back("multipart/form-data");
    ContentType.push_back("text/css");
    ContentType.push_back("text/csv");
    ContentType.push_back("text/html");
    ContentType.push_back("text/javascript");
    ContentType.push_back("text/plain");
    ContentType.push_back("text/xml");
    ContentType.push_back("video/mpeg");
    ContentType.push_back("video/mp4");
    ContentType.push_back("video/quicktime");
    ContentType.push_back("video/x-ms-wmv");
    ContentType.push_back("video/x-msvideo");
    ContentType.push_back("video/x-flv");
    ContentType.push_back("video/webm");
    ContentType.push_back("application/vnd.android.package-archive");
    ContentType.push_back("application/vnd.oasis.opendocument.text");
    ContentType.push_back("application/vnd.oasis.opendocument.spreadsheet");
    ContentType.push_back("application/vnd.oasis.opendocument.presentation");
    ContentType.push_back("application/vnd.oasis.opendocument.graphics");
    ContentType.push_back("application/vnd.ms-excel");
    ContentType.push_back("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
    ContentType.push_back("application/vnd.ms-powerpoint");
    ContentType.push_back("application/vnd.openxmlformats-officedocument.presentationml.presentation");
    ContentType.push_back("application/msword");
    ContentType.push_back("application/vnd.openxmlformats-officedocument.wordprocessingml.document");
    ContentType.push_back("application/vnd.mozilla.xul+xml");
}