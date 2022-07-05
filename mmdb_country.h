static void load_arguments(int argc, char **argv, char **mmdb_file, char **ip_address);
static void usage(char *program, int exit_code, const char *error);
static MMDB_s open_or_die(const char *mmdb_filename);
static int lookup_result(MMDB_s *mmdb, const char *ip_address, MMDB_lookup_result_s *result);
static int print_country(MMDB_lookup_result_s *lookup_result, const char *ip_address);

#define MMDB_COUNTRY_SUCCESS 0
#define MMDB_COUNTRY_ARGUMENT_ERROR 1
#define MMDB_COUNTRY_DB_LOAD_ERROR 2
#define MMDB_COUNTRY_GETADDRESSINFO_ERROR 3
#define MMDB_COUNTRY_LIBMAXMINDDB_ERROR 4
#define MMDB_COUNTRY_NO_COUNTRY_NAME_ERROR 5
#define MMDB_COUNTRY_NO_IP_FOUND_ERROR 6
