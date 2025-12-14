#include <iostream>
#include <getopt.h>

void opts(int argc, char* argv[]) {
  int c;
  int digit_optind = 0;

  while (1) {
    int cur_opt_ind = optind ? optind : 1;
    int option_index = 0;
  
    static struct option long_options[] = {
      { "recursive",    0,  0,  'r' },
      { "delete",       0,  0,  'd' },
      { "input",        1,  0,  'i' },
      { "output",       2,  0,  'o' },
      {0,               0,  0,   0  }
    };
  
    c = getopt_long(argc, argv, "rdi:o::", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 0:
        std::cout << "option " << long_options[option_index].name;
        if (optarg)
          std::cout << " with arg " << optarg << std::endl;
        break;

      case '0':
      case '1':
      case '2':
        if (digit_optind != 0 && digit_optind != cur_opt_ind)
          std::cout << "Digits occur in two different argv-elements." << std::endl;
        digit_optind = cur_opt_ind;
        std::cout << "option" << c << std::endl;

      case 'r':
        std::cout << "option r" << std::endl;
        break;
      case 'd':
        std::cout << "option d" << std::endl;
        break;
      case 'o':
        std::cout << "option o with val " << optarg << std::endl;
        break;
      case 'i':
        std::cout << "option i with val " << optarg << std::endl;
        break;
      case '?':
        break;

      default:
        std::cout << "?? getopt returned character code " << c << std::endl;
        abort();
    }
  }
}
