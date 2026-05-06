/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_message.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitaabe <haitaabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 by haitaabe                   #+#    #+#             */
/*   Updated: 2026/04/18 by haitaabe                   ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/channels/channel.hpp"
#include "../../include/utls.hpp"
#include "../../include/server.hpp"

void managerchannel::handlePrivmsg(const std::string &input, client &c)
{
    if (!c.regestred)
    {
        std::string err = ":ircserv 451 " + (c.nickname.empty() ? "*" : c.nickname) + " :You have not registered\r\n";
        send(c.fd, err.c_str(), err.size(), 0);
        return;
    }

    std::stringstream ss(input);
    std::string command, allTargets;
    ss >> command >> allTargets;
    size_t colon_pos = input.find(':');
    if (colon_pos == std::string::npos || allTargets.empty())
    {
        std::string err = ":ircserv 412 " + c.nickname + " :No text to send\r\n";
        send(c.fd, err.c_str(), err.size(), 0);
        return;
    }
    std::string message_content = input.substr(colon_pos);

    std::vector<std::string> targets = splitByComma(allTargets);

    for (size_t t = 0; t < targets.size(); ++t)
    {
        std::string target = targets[t];

        std::string full_msg = ":" + c.nickname + " PRIVMSG " + target + " " + message_content + "\r\n";
        if (full_msg.size() > 512)
            full_msg = full_msg.substr(0, 510) + "\r\n";

        if (target[0] == '#')
        {
            std::map<std::string, Channel *>::iterator it_chan = channels.find(target);

            if (it_chan != channels.end())
            {
                Channel *room = it_chan->second;

                bool is_member = false;
                for (size_t i = 0; i < room->members.size(); i++)
                {
                    if (room->members[i] == c.fd)
                    {
                        is_member = true;
                        break;
                    }
                }

                if (!is_member)
                {
                    std::string err = ":ircserv 404 " + c.nickname + " " + target + " :Cannot send to channel\r\n";
                    send(c.fd, err.c_str(), err.size(), 0);
                    continue;
                }

                for (size_t i = 0; i < room->members.size(); i++)
                {
                    if (room->members[i] != c.fd)
                    {
                        send(room->members[i], full_msg.c_str(), full_msg.size(), 0);
                    }
                }
            }
            else
            {

                std::string err = ":ircserv 401 " + c.nickname + " " + target + " :No such nick/channel\r\n";
                send(c.fd, err.c_str(), err.size(), 0);
            }
        }
        else
        {
            bool found_and_active = false;

            for (std::map<int, client>::iterator it_cli = _clients.begin(); it_cli != _clients.end(); ++it_cli)
            {
                if (it_cli->second.nickname == target)
                {

                    if (it_cli->second.regestred == true)
                    {
                        send(it_cli->first, full_msg.c_str(), full_msg.size(), 0);
                        found_and_active = true;
                    }

                    break;
                }
            }

            if (!found_and_active)
            {
                std::string err = ":ircserv 401 " + c.nickname + " " + target + " :No such nick/channel\r\n";
                send(c.fd, err.c_str(), err.size(), 0);
            }
        }
    }
}