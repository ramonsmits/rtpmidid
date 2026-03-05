/**
 * Real Time Protocol Music Instrument Digital Interface Daemon
 * Copyright (C) 2019-2023 David Moreno Montero <dmoreno@coralbits.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "network_rtpmidi_multi_listener.hpp"
#include "factory.hpp"
#include "json.hpp"
#include "midirouter.hpp"
#include "rtpmidid/mdns_rtpmidi.hpp"
#include "utils.hpp"

namespace rtpmididns {

extern std::shared_ptr<::rtpmidid::mdns_rtpmidi_t> mdns;

network_rtpmidi_multi_listener_t::network_rtpmidi_multi_listener_t(
    const std::string &name, const std::string &port,
    std::shared_ptr<aseq_t> aseq_, bool merge_network_input_,
    bool merge_network_output_)
    : aseq(aseq_), server(name, port),
      merge_network_input(merge_network_input_),
      merge_network_output(merge_network_output_) {
  if (mdns)
    mdns->announce_rtpmidi(name, server.port());

  status_change_connection = server.status_change_event.connect(
      [this](std::shared_ptr<rtpmidid::rtppeer_t> peer,
             rtpmidid::rtppeer_t::status_e status) {
        if (status != rtpmidid::rtppeer_t::status_e::CONNECTED) {
          return;
        }
        DEBUG("Got connection from {}", peer->remote_name);

        midipeer_id_t alsa_id;
        if (merge_network_input) {
          if (shared_alsa_peer_id == 0) {
            shared_alsa_peer_id =
                router->add_peer(make_local_alsa_peer(server.name, aseq));
            DEBUG("Created shared ALSA peer {} for merged network input",
                  shared_alsa_peer_id);
          }
          alsa_id = shared_alsa_peer_id;
        } else {
          alsa_id =
              router->add_peer(make_local_alsa_peer(peer->remote_name, aseq));
        }

        auto peer_id = router->add_peer(make_network_rtpmidi_peer(peer));
        router->connect(alsa_id, peer_id);
        router->connect(peer_id, alsa_id);

        if (merge_network_output && !merge_network_input) {
          if (shared_alsa_output_peer_id == 0) {
            shared_alsa_output_peer_id =
                router->add_peer(make_local_alsa_peer(server.name, aseq));
            DEBUG("Created shared broadcast ALSA peer {} for merged network "
                  "output",
                  shared_alsa_output_peer_id);
          }
          router->connect(shared_alsa_output_peer_id, peer_id);
        }
      });
}

void network_rtpmidi_multi_listener_t::send_midi(midipeer_id_t from,
                                                 const mididata_t &) {}

json_t network_rtpmidi_multi_listener_t::status() {
  std::vector<json_t> peers;
  for (auto &peer : server.peers) {
    auto &peerpeer = peer.peer;
    peers.push_back(peer_status(*peerpeer));
  }

  return json_t{
      //
      {"peers", peers},      //
      {"name", server.name}, //
      {"listening",
       {
           //
           {"name", server.name},           //
           {"control_port", server.port()}, //
           {"midi_port", server.port() + 1} //
       }} //
  };
}

} // namespace rtpmididns
