/*
 * casocklib - An asynchronous communication library for C++
 * ---------------------------------------------------------
 * Copyright (C) 2010 Leandro Costa
 *
 * This file is part of casocklib.
 *
 * casocklib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * casocklib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with casocklib. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \file casock/proactor/asio/server/SocketServer.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/proactor/asio/server/SocketServer.h"

#include <boost/bind.hpp>

#include "casock/util/Logger.h"
#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "casock/proactor/asio/server/SocketSession.h"

namespace casock {
  namespace proactor {
    namespace asio {
      namespace server {
        SocketServer::SocketServer (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const unsigned short& port)
          : mrAsyncProcessor (rAsyncProcessor), m_acceptor (mrAsyncProcessor.service (), ::asio::ip::tcp::endpoint (::asio::ip::tcp::v4 (), port))
        {
          LOGMSG (LOW_LEVEL, "SocketServer::SocketServer () - port [%hu]\n", port);
        }

        void SocketServer::asyncAccept ()
        {
          LOGMSG (LOW_LEVEL, "SocketServer::%s ()\n", __FUNCTION__);
          SocketSession* pSession = buildSession (mrAsyncProcessor);

          LOGMSG (LOW_LEVEL, "SocketServer::%s () - calling async acceptor...\n", __FUNCTION__);
          m_acceptor.async_accept (pSession->socket (), ::boost::bind (&SocketServer::handle_accept, this, pSession, ::asio::placeholders::error));
        }

        void SocketServer::handle_accept (SocketSession* pSession, const ::asio::error_code& error)
        {
          if (! error)
          {
            LOGMSG (LOW_LEVEL, "SocketServer::%s () - NO ERROR!\n", __FUNCTION__);
            pSession->onConnect ();
          }
          else
          {
            LOGMSG (LOW_LEVEL, "SocketServer::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
            pSession->onConnectionFailure ();
          }

          asyncAccept ();
        }

        void SocketServer::start ()
        {
          LOGMSG (LOW_LEVEL, "SocketServer::%s ()\n", __FUNCTION__);

          asyncAccept ();
        }
      }
    }
  }
}