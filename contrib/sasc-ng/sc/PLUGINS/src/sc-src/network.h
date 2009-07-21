/*
 * Softcam plugin to VDR (C++)
 *
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * Or, point your browser to http://www.gnu.org/copyleft/gpl.html
 */

#ifndef ___NETWORK_H
#define ___NETWORK_H

#include <vdr/thread.h>
#include <vdr/tools.h>
#include "misc.h"

// ----------------------------------------------------------------

#define DEFAULT_CONNECT_TIMEOUT   20
#define DEFAULT_READWRITE_TIMEOUT 3
#define DEFAULT_IDLE_TIMEOUT      120

extern const char *netscript;
extern int netTimeout;

// ----------------------------------------------------------------

class cNetWatcher;

class cNetSocket : public cSimpleItem, private cMutex {
friend class cNetWatcher;
private:
  int sd;
  char *hostname;
  int port, dummy, conTimeout, rwTimeout, idleTimeout;
  bool udp, connected, netup;
  cTimeMs activity;
  //
  int Select(bool forRead, int timeout);
  void Activity(void);
  bool GetAddr(struct sockaddr_in *saddr, const char *Hostname, int Port);
  int GetSocket(bool Udp);
public:
  cNetSocket(int ConnectTimeout, int ReadWriteTimeout, int IdleTimeout, bool Udp=false);
  ~cNetSocket();
  bool Connect(const char *Hostname, int Port, int timeout=-1);
  bool Bind(const char *Hostname, int Port);
  void Disconnect(void);
  int Read(unsigned char *data, int len, int timeout=-1);
  int Write(const unsigned char *data, int len, int timeout=-1);
  int SendTo(const char *Host, int Port, const unsigned char *data, int len, int timeout=-1);
  void Flush(void);
  bool Connected(void) { return connected; }
  };

#endif //___NETWORK_H
