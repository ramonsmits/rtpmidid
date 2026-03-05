% RTPMIDID(1) rtpmidid 21.11
% David Moreno <dmoreno@coralbits.com>
% November 2021

# NAME

rtpmidid -- RTP MIDI daemon that exports via ALSA sequencer interface

# SYNOPSIS

**rtpmidid** _\[options\]_

# DESCRIPTION

`rtpmidid` allows you to share ALSA sequencer devices on the network using RTP
MIDI, and import other network shared RTP MIDI devices.

`rtpmidid` is an user daemon, and when a RTP MIDI device is announced using mDNS
(also known as Zeroconf, Avahi, and multicast DNS) it exposes this ALSA
sequencer port.

# OPTIONS

**\--ini** _path_
: Loads an INI file as default configuration. Depending on order may overwrite
other arguments. See **INI FILE** below.

**\--port** _port_
: Opens local port as server. Default 5004.

**\--name** _name_
: Forces the ALSA and rtpmidi name.

**\--alsa-name** _name_
: Forces the ALSA name only.

**\--rtpmidid-name** _name_
: Forces the rtpmidi name only.

**\--control** _path_
: Creates a control socket at the given path. See **docs/CONTROL.md**.
Default `/var/run/rtpmidid/control.sock`.

**\--log-level** _level_
: Set log level. Accepted values: `debug`, `info`, `warning`, `error` (or `0`-`3`).
Default: `info`.

**\--rtpmidi-discover** _value_
: Enable or disable rtpmidi mDNS discovery. Accepted values: `true`, `false`,
a positive regex, or `!`_negregex_ (prefixed with `!` for negative match).

**\--rawmidi** _device_
: Connects to a raw MIDI device. For example `/dev/snd/midiC1D0`.

**\--version**
: Show version and exit.

**\--help**
: Show help and exit.

# INI FILE

The INI file configures the daemon's behavior. It supports the following
sections. Sections marked with **(repeatable)** can appear multiple times.

## [general]

**alsa_name**=_name_
: Name for the ALSA sequencer client. Default: `rtpmidid`.

**control**=_path_
: Path for the control socket. Default: `/var/run/rtpmidid/control.sock`.

**log_level**=_level_
: Log level: `debug`, `info`, `warning`, `error`. Default: `info`.

## [rtpmidi_announce] (repeatable)

Creates an RTP MIDI server endpoint announced via mDNS.

**name**=_name_
: Name for the announced service. Use `{{hostname}}` for the machine hostname.

**port**=_port_
: UDP port to listen on. Default: `5004`.

## [alsa_announce] (repeatable)

Creates an ALSA sequencer port. When a local ALSA client connects, an RTP MIDI
service is announced on the network.

**name**=_name_
: Name for the ALSA port.

## [connect_to] (repeatable)

Connects to a remote RTP MIDI peer on startup.

**hostname**=_host_
: Hostname or IP address of the remote peer.

**port**=_port_
: UDP port of the remote peer. Default: `5004`.

**name**=_name_
: Local name for the connection.

**local_udp_port**=_port_
: Local UDP port to use. Default: random.

## [rtpmidi_discover]

Controls automatic discovery of mDNS-announced RTP MIDI services.

**enabled**=_bool_
: Enable or disable discovery. Default: `true`.

**name_positive_regex**=_regex_
: Only accept services matching this regex. Default: `.*` (accept all).

**name_negative_regex**=_regex_
: Reject services matching this regex. Default: `^$` (reject nothing).

The name checked is in the format `server:port/service`. The check order is:
first reject if negative matches, then accept if positive matches, otherwise
reject.

## [alsa_hw_auto_export]

Automatically export hardware ALSA MIDI devices to the network.

**type**=_type_
: Which devices to export: `none`, `hardware`, `software`, `all`. Default: `none`.

**name_positive_regex**=_regex_
: Only export devices matching this regex.

**name_negative_regex**=_regex_
: Exclude devices matching this regex.

## [rawmidi] (repeatable)

Export a raw MIDI device (e.g. `/dev/snd/midiC1D0`) to the network.

**device**=_path_
: Path to the raw MIDI device. Required.

**name**=_name_
: Name for the exported device.

**hostname**=_host_
: Set to a hostname to connect as client. Leave empty or `0.0.0.0` to listen as server.

**local_udp_port**=_port_
: Local UDP port.

**remote_udp_port**=_port_
: Remote UDP port (when connecting as client).

# EXAMPLES

**rtpmidid \--ini /etc/rtpmidid/default.ini**

Start with the default configuration file.

**rtpmidid \--port 5004 \--name instruments**

Listen on port 5004 with the name "instruments".

**rtpmidid \--ini default.ini \--log-level warning**

Load configuration then override the log level to only show warnings and errors.

# EXIT VALUES

**0**
: Success

**1**
: Error starting

# FILES

*/etc/rtpmidid/default.ini*
: Default system configuration file.

*~/.config/rtpmidid/default.ini*
: User configuration file (when running as a user service).

*/var/run/rtpmidid/control.sock*
: Default control socket (system service).

*$XDG_RUNTIME_DIR/rtpmidid/control.sock*
: Control socket (user service).

# SEE ALSO

**rtpmidid-cli(1)**, **docs/CONTROL.md**

# COPYRIGHT

(C) 2019-2023 David Moreno Montero <dmoreno@coralbits.com>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
