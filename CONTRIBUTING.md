# CONTRIBUTING

If you want to contribute, please create a pull request or issue at
https://github.com/davidmoreno/rtpmidid

Create a pull request if you have some code to show, even if it's not ready to
merge, so we can discuss it, and improve it.

If you have some idea you want to comment, but no code to show, create an
issue, and we can later create a related pull request if needed.

## Documentation checklist

When adding or changing features, update all relevant documentation:

- **Man pages** (`rtpmidid.1.md`, `rtpmidid-cli.1.md`) — update if you add/change
  command-line options, INI settings, or control socket commands.
- **README.md** — update if the change affects user-visible behavior, configuration
  examples, or the feature roadmap.
- **docs/CONTROL.md** — update if you add/change control socket commands.
- **default.ini** — add commented examples for new INI settings.
- **Tests** — add or update tests in `tests/` for new settings, router behavior,
  or INI parsing.

Man pages are built with `make man` (requires `pandoc`).

Please note that when contributing code you agree that this code is GPLv3
licensed.

# CONTRIBUTORS

* David Moreno - https://github.com/davidmoreno - Creator
* Albert Graef - https://github.com/agraef - SysEx support, added more MIDI messages
* sadguitarius - https://github.com/sadguitarius - Build fixes, mold linker fix, long SysEx, ALSA multi listener fix
* Moritz Müller-Guthof - https://github.com/momomentum - Remove global mDNS pointer hack
* Ramon Smits - https://github.com/ramonsmits - Merge network input/output, user service install, man pages, documentation
