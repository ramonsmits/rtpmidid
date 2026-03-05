% RTPMIDID-CLI(1) rtpmidid 21.11
% David Moreno <dmoreno@coralbits.com>
% November 2021

# NAME

rtpmidid-cli -- Command line interface for a running rtpmidid

# SYNOPSIS

**rtpmidid-cli** _\[socket-path\]_ _command_ \[_params..._\] \[**.** _command_ \[_params..._\]\]

# DESCRIPTION

Communicate with a running **rtpmidid(1)** instance via its Unix control socket.
Allows inspecting state, creating connections, and managing peers at runtime.

Responses are always JSON, for easy parsing by external programs.

Multiple commands can be given in one invocation, separated by **.** (a single dot).

The control socket path defaults to `/var/run/rtpmidid/control.sock`. When
running as a user service, use `$XDG_RUNTIME_DIR/rtpmidid/control.sock`.

# COMMANDS

**help**
: List all available commands with descriptions.

**status**
: Show daemon status: version, settings, router peers, mDNS announcements, and
latency information.

**connect** _name_ \[_host_\] \[_port_\]
: Connect to a remote RTP MIDI server. Only _name_ is required. Default host
is the same as _name_, default port is `5004`. Host must be specified to use
a different port.

**router.remove** _peer\_id_
: Remove a peer from the router.

**router.connect** _from_ _to_
: Create a unidirectional connection between two router peers.

**router.disconnect** _from_ _to_
: Remove a unidirectional connection between two router peers.

**router.create** _params_
: Create a new peer. Pass `{"type": "list"}` to see available peer types and
their parameters.

**mdns.remove** _params_
: Delete an mDNS announcement. Params: `{"name": ..., "hostname": ..., "port": ...}`.

**export.rawmidi** _params_
: Export a raw MIDI device. Params: `{"device": ..., "name": ..., "hostname": ...}`.
Pass without `device` to see parameter help.

# EXAMPLES

**rtpmidid-cli help**

List available commands.

**rtpmidid-cli status**

Show current daemon status.

**rtpmidid-cli connect deepmind12 192.168.1.35 5004**

Connect to IP 192.168.1.35 port 5004 with local name "deepmind12".

**rtpmidid-cli connect deepmind12 192.168.1.35 5004 . connect win10 192.168.1.36 . connect mac.local**

Create three connections in one invocation.

**rtpmidid-cli $XDG_RUNTIME_DIR/rtpmidid/control.sock status**

Query status of a user service instance.

# PROTOCOL

The underlying protocol is JSON-RPC over a Unix stream socket. Each request is
a JSON object with a `method` key and optional `params`:

    echo '{"method":"status"}' | socat - UNIX-CONNECT:/var/run/rtpmidid/control.sock

The response is a JSON object with either a `result` or `error` key.

# EXIT VALUES

**0**
: Success

**1**
: Error

# SEE ALSO

**rtpmidid(1)**, **docs/CONTROL.md**

# COPYRIGHT

(C) 2019-2023 David Moreno Montero <dmoreno@coralbits.com>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
