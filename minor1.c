#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// represents the flag that was passed via the command line
// decides whether this program will search for URLs, emails or phone numbers.
typedef enum {URL = 0, EMAIL = 1, PHONE = 2} flag_t;

// maps flag indices to corresponding regexes that should be used
// regex[URL] will evaluate to the regex for URLs (since URL = 0)
char const *regex_for_flag[] = {
    // URL
    "(http|https)\\:\\/\\/[a-z0-9\\-\\_]+\\.unt\\.edu((\\.|\\/)?[a-z0-9\\_\\?\\=\\-\\+\\@\\,\\#\\!\\$\\&\\'\\(\\)\\*\\;]+)+",
    // EMAIL
    "([a-z0-9\\_\\-\\.]+\\@unt.edu)",
    // PHONE
    "([0-9]{3}\\-){2}[0-9]{4}",
};

// this struct stores all arguments that were passed via the command line,
// including the flag and the input filename.
typedef struct
{
    flag_t flag;
    char const *input_file;
} args_t;

// print a message before exiting the program with error code 1.
void fatal_error(char const *msg)
{
    printf("%s", msg);
    exit(EXIT_FAILURE);
}

// parse the arguments that were passed to main()
// and create the corresponding arguments struct
args_t read_args(int argc, char *argv[])
{
    char const *usage = "Usage: \n./minor1 [-url | -email | -phone] input_file\n";
    // print usage if the argument count is not two.
    // argc also counts the invoked program filename as an argument
    if (argc != 3)
        fatal_error(usage);

    args_t args;

    // parse the flag. strcmp will return zero if the strings are equal
    if (!strcmp(argv[1], "-url"))
        args.flag = URL;
    else if (!strcmp(argv[1], "-email"))
        args.flag = EMAIL;
    else if (!strcmp(argv[1], "-phone"))
        args.flag = PHONE;
    else // invalid flag, print usage.
        fatal_error(usage);

    // "parse" input filename
    args.input_file = argv[2];
    return args;
}

int main(int argc, char *argv[])
{
    args_t args = read_args(argc, argv);
    // the choice of regex depends on the flag that was passed
    char const *regex = regex_for_flag[args.flag];

    // split this program into two new processes:
    // one instance will call egrep to search for matching text
    // the other instance will call uniq to remove duplicate lines
    // the output of egrep is bound to fd[1],
    // uniq will read from fd[0]
    int fd[2];
    // create a pipe to connect fd[0] and fd[1],
    // such that the output of egrep is sent to uniq
    pipe(fd);

    // split the program. fork() will return 0 for the child,
    // and thus the first branch will be executed for the child.
    if (!fork())
    {
        // close reading descriptor, not used by child process
        close(fd[0]);
        // connect writing descriptor with stdout
        // to send the output of egrep to uniq
        dup2(fd[1], 1);
        // close unused file descriptor
        close(fd[1]);

        // execute egrep now, which will send its output to fd[1] --> uniq
        execlp("egrep",
                "egrep", // use eqrep
                "-oi", // do not display whole lines and ignore case
                regex, args.input_file,
                (char *)0); // null termination for argument list
    }
    // this branch is executed for the parent that calls uniq
    else
    {
        // close writing descriptor, not used by parent process
        close(fd[1]);
        // connect reading descriptor to stdin
        // to read the output from egrep
        dup2(fd[0], 0);
        // close unused file descriptor
        close(fd[0]);

        // execute uniq now, which will remove duplicates and print them out.
        execlp("uniq", "uniq", (char *)0);
    }
}
