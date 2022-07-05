# mmdb_country
Returns the country name for a given IP address.

Based in no small part on the mmdblookup program from https://github.com/maxmind/libmaxminddb

## Dependencies
Before building, you'll need to fetch the [libmaxminddb](https://github.com/maxmind/libmaxminddb) library.

### Ubuntu via PPA
MaxMind provides a PPA for recent version of Ubuntu. To add the PPA to your APT sources, run:

```sh
$ sudo add-apt-repository ppa:maxmind/ppa
```

Then install the packages by running:

```sh
$ sudo apt update
$ sudo apt install libmaxminddb0
```

### Mac via Homebrew
You can install libmaxminddb on macOS using [Homebrew](https://brew.sh/):

```sh
$ brew install libmaxminddb
```

## Building
```sh
make
```

## Usage
```sh
$ mmdb_country --file /path/to/file.mmdb --ip 1.2.3.4
Australia
```

This application returns the country associated with an IP. It accepts the following options:

    --file (-f)     The path to the MMDB file. Required.
    --ip (-i)       The IP address to look up. Required.
    --help (-h -?)  Show this usage information.

## Getting the MaxMind DB
This program should work equally well with the commercial licenced GeoIP2 (higher accuracy/paid)
or GeoLite2 (lower accuracy/free) datasets. Given we're just looking at country, the decision over which
dataset to use will likely come down to how you plan to use the data.

Such as https://dev.maxmind.com/geoip/geolite2-free-geolocation-data?lang=en

The Country based dataset (in mmdb format) would be the most appropriate.
The City dataset will work, but it is more than 10 times the size
(although the runtime seem to be pretty much the same for both).
