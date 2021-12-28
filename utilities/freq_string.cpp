/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "freq_string.hpp"

uint32_t freq_string_sections(const std::string &str)
{
    std::string::size_type length = str.length();

    if (length == 0) {
        return 0;
    }

    char next = str.at(0);
    uint32_t sections = 0;
    std::string::size_type pos = 0;

    while ((pos = str.find(';',pos + 1)) != std::string::npos) {
        if (pos < length - 1) {
            next = str.at(pos + 1);
            if ((next == ',') || (next == ';')) {
                RD_THROW rd::syntax_error("Invalid string : \""
                                             + str
                                             + "\",with continuously separator.");
            }
        }
        sections ++;
    }

    while ((pos = str.find(',',pos + 1)) != std::string::npos) {
        if (pos < length - 1) {
            next = str.at(pos + 1);
            if ((next == ',') || (next == ';')) {
                RD_THROW rd::syntax_error("Invalid string : \""
                                             + str
                                             + "\",with continuously separator.");
            }
        }
        sections ++;
    }

    if ((str.back() != ';') && (str.back() != ',')) {
        sections ++;
    }

    return sections;
}

uint32_t freq_string_sections(const char *str)
{
    size_t length = strlen(str);

    RD_ASSERT_THROW(length > 0);

    const char *str_temp = str;
    const char back = *(str_temp + length - 1);
    char next = '\0';
    uint32_t sections = 0;

    while ((str_temp = strpbrk(str_temp,";"))) {          // Find all ";"
        if (size_t(str_temp - str) < length - 1) {
            next = *(str_temp + 1);
            if ((next == ',') || (next == ';')) {
                RD_THROW rd::syntax_error("Invalid string : \""
                                             + std::string(str)
                                             + "\",with continuously separator.");
            }
        }
        sections ++;
        str_temp ++;
    }

    str_temp = str;
    while ((str_temp = strpbrk(str_temp,","))) {          // Find all ","
        if (size_t(str_temp - str) < length - 1) {
            next = *(str_temp + 1);
            if ((next == ',') || (next == ';')) {
                RD_THROW rd::syntax_error("Invalid string : \""
                                             + std::string(str)
                                             + "\",with continuously separator.");
            }
        }
        sections ++;
        str_temp ++;
    }

    if ((back != ';') && (back != ',')) {
        sections ++;
    }

    return sections;
}

void split_freq_string(const char *str_in, uint32_t sections, char (*str_out)[32])
{
    size_t length = strlen(str_in);

    RD_ASSERT_THROW(length > 0);

    const char *str_in_temp = str_in;
    uint32_t pos[2] = {0,0};

    for (uint32_t i = 0;i < sections;i ++) {
        while (*str_in == ' ' || *str_in == '\t') {
            str_in ++;
        }

        str_in_temp = strchr(str_in,';');
        pos[0] = (str_in_temp != nullptr) ? uint32_t(str_in_temp - str_in) : 0;

        str_in_temp = strchr(str_in,',');
        pos[1] = (str_in_temp != nullptr) ? uint32_t(str_in_temp - str_in) : 0;

        if (pos[0] > 0) {
            pos[0] = pos[1] > 0 ? std::min(pos[0],pos[1]) : pos[0];
        }
        else {
            pos[0] = pos[1] > 0 ? pos[1] : uint32_t(strlen(str_in));
        }

        for (uint32_t j = 0;j < pos[0];j ++) {
            str_out[i][j] = str_in[j];
        }
        str_out[i][pos[0]] = '\0';

        str_in += pos[0] + 1;
    }
}
