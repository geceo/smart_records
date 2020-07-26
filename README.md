# This repo holds WIP, not usable "as-is" for the moment !!

# smart_records
A small library to write "records" with MySQL, CSV, UDP, Multicast and SQLite backends

# Why ?
Originally, i just wanted to add support of MySQL instead of CSV files to `rtl_power` and `rx_power` tools.

To avoid code duplication, etc... i decided to write a small and simple wrapper library with optionnal support for MySQL (libmysqlclient), CSV (internal).
Work is in progress to implement UDP (internal), Multicast (internal) and SQLite backends.

I wanted it to be easy to add in any tools so:

* No configuration file, everything information needed is passed with a single URI.
* No huge runtime dependencies.
* Optionnal backends (so that you can just embed what you need).

