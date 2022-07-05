// mmdb_country.c
//
// Based in no small part on the mmdblookup program from https://github.com/maxmind/libmaxminddb
//
// Opens a mmdb file, looks up an IP address, then displays the associated country


#include "maxminddb.h"
#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>

#include "mmdb_country.h"

int main(int argc, char **argv) {
  char *mmdb_filename = NULL;
  char *ip_address = NULL;

  // Load arguments
  load_arguments(argc, argv, &mmdb_filename, &ip_address);

  // Open the DB
  MMDB_s mmdb = open_or_die(mmdb_filename);

  // Lookup the IP address
  MMDB_lookup_result_s result;
  int exit_code = lookup_result(&mmdb, ip_address, &result);

  // Print the country
  if (exit_code == MMDB_COUNTRY_SUCCESS) {
    exit_code = print_country(&result, ip_address);
  }

  // Clean up the DB access
  MMDB_close(&mmdb);

  return exit_code;
}

static void load_arguments(int argc, char **argv, char **mmdb_filename, char **ip_address) {
  int help = 0;
  char *program_name = basename(argv[0]);
  int opt_index, opt_char;
  static struct option options[] = {
    { "file", required_argument, 0, 'f' },
    { "ip", required_argument, 0, 'i' }
  };

  while ((opt_char = getopt_long(argc, argv, "f:i:", options, &opt_index)) != -1) {
    switch (opt_char) {
      case 'f':
        *mmdb_filename = optarg;
        break;
      case 'i':
        *ip_address = optarg;
        break;
      default:
        help = 1;
    }
  }

  if (help) {
    usage(program_name, MMDB_COUNTRY_SUCCESS, NULL);
  }

  if (*mmdb_filename == NULL) {
    usage(program_name, MMDB_COUNTRY_ARGUMENT_ERROR, "You must profile a MaxMind DB file with --file");
  }

  if (*ip_address == NULL) {
    usage(program_name, MMDB_COUNTRY_ARGUMENT_ERROR, "You must provide an IP address with --ip");
  }  
}

static void usage(char *program, int exit_code, const char *error) {
  if (error != NULL) {
    fprintf(stderr, "*ERROR: %s\n", error);
  }

  char *usage =
    "%s --file /path/to/file.mmdb --ip 1.2.3.4\n"
    "\n"
    "This application returns the country associated with an IP. It accepts the following options:\n"
    "\n"
    "    --file (-f)     The path to the MMDB file. Required.\n"
    "    --ip (-i)       The IP address to look up. Required.\n"
    "    --help (-h -?)  Show usage information.\n"
    "\n";

  fprintf(stdout, usage, program);
  exit(exit_code);
}

static MMDB_s open_or_die(const char *mmdb_filename) {
  MMDB_s mmdb;
  int status = MMDB_open(mmdb_filename, MMDB_MODE_MMAP, &mmdb);

  switch (status) {
    case MMDB_IO_ERROR:
      fprintf(stderr, "IO error: %s\n", strerror(errno));
      break;
    case MMDB_SUCCESS:
      return mmdb;
    default:
      fprintf(stderr, "Can't open %s - %s\n", mmdb_filename, MMDB_strerror(status));
  }
  exit(MMDB_COUNTRY_DB_LOAD_ERROR);
}

static int lookup_result(MMDB_s *mmdb, const char *ip_address, MMDB_lookup_result_s *result) {
  int gai_error, mmdb_error;
  *result = MMDB_lookup_string(mmdb, ip_address, &gai_error, &mmdb_error);

  if (gai_error != 0) {
    char const *const strerr = gai_strerror(gai_error);
    fprintf(stderr, "Error from call to getaddrinfo for %s - %s\n", ip_address, strerr);
    return MMDB_COUNTRY_GETADDRESSINFO_ERROR;
  }

  if (mmdb_error != MMDB_SUCCESS) {
    fprintf(stderr, "Got an error from the maxminddb library: %s\n", MMDB_strerror(mmdb_error));
    return MMDB_COUNTRY_LIBMAXMINDDB_ERROR;
  }

  return MMDB_COUNTRY_SUCCESS;
}

static int print_country(MMDB_lookup_result_s *lookup_result, const char *ip_address) {
  if (lookup_result->found_entry) {
    MMDB_entry_data_s entry_data;
    int status = MMDB_get_value(&lookup_result->entry, &entry_data, "country", "names", "en", NULL);

    if (status == MMDB_SUCCESS && entry_data.has_data && entry_data.type == MMDB_DATA_TYPE_UTF8_STRING) {
      char *country = strndup(entry_data.utf8_string, entry_data.data_size);
      fprintf(stdout, "%s\n", country);
      return MMDB_COUNTRY_SUCCESS;
    } else {
      fprintf(stderr, "Could not find a country name for this IP address (%s)\n", ip_address);
      return MMDB_COUNTRY_NO_COUNTRY_NAME_ERROR;
    }
  } else {
    fprintf(stderr, "Could not find an entry for this IP address (%s)\n", ip_address);
    return MMDB_COUNTRY_NO_IP_FOUND_ERROR;
  }
}
