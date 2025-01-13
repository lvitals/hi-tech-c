#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

#define MAXLIST 60

// Flags globais
static bool preprocess = false;
static bool compile_flag = false;
static bool generate_asm = false;
static bool optimize = false;
static bool assemble = false;
static bool link_flag = false;
static bool verbose = false;

// Vetores para armazenar arquivos e caminhos
static char *input_files[MAXLIST];
static char *libraries[MAXLIST];
static size_t input_idx = 0;
static size_t lib_idx = 0;

// Funções auxiliares
static void error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

static void print_verbose(const char *message) {
    if (verbose) {
        printf("[VERBOSE]: %s\n", message);
    }
}

static void parse_arguments(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (toupper(argv[i][1])) {
                case 'C':
                    compile_flag = true;
                    break;
                case 'O':
                    optimize = true;
                    break;
                case 'I':
                    preprocess = true;
                    break;
                case 'F':
                    generate_asm = true;
                    break;
                case 'S':
                    assemble = true;
                    break;
                case 'X':
                    link_flag = true;
                    break;
                case 'V':
                    verbose = true;
                    break;
                case 'L': // Biblioteca
                    if (lib_idx >= MAXLIST) {
                        error("Too many libraries specified");
                    }
                    libraries[lib_idx++] = argv[i];
                    break;
                default:
                    fprintf(stderr, "Unknown option: %s\n", argv[i]);
                    exit(EXIT_FAILURE);
            }
        } else {
            if (input_idx >= MAXLIST) {
                error("Too many input files");
            }
            input_files[input_idx++] = argv[i];
        }
    }
}

static void preprocess_files() {
    for (size_t i = 0; i < input_idx; ++i) {
        char command[512];
        snprintf(command, sizeof(command), "../cpp/cpp -DCPM -DHI_TECH_C -Dz80 -I %s %s.tmp", input_files[i], input_files[i]);
        print_verbose("Preprocessing...");
        printf("Executing: %s\n", command);
        if (system(command) != 0) {
            error("Preprocessing failed");
        }
    }
}

static void compile_files() {
    for (size_t i = 0; i < input_idx; ++i) {
        char command[512];
        snprintf(command, sizeof(command), "../p1/p1 %s.tmp %s2.tmp", input_files[i], input_files[i]);
        print_verbose("Compiling...");
        printf("Executing: %s\n", command);
        if (system(command) != 0) {
            error("Compilation failed");
        }
    }
}

static void generate_assembly() {
    for (size_t i = 0; i < input_idx; ++i) {
        char command[512];
        snprintf(command, sizeof(command), "../cgen/cgen %s2.tmp %s1.tmp", input_files[i], input_files[i]);
        print_verbose("Generating assembly...");
        printf("Executing: %s\n", command);
        if (system(command) != 0) {
            error("Assembly generation failed");
        }
    }
}

static void optimize_files() {
    for (size_t i = 0; i < input_idx; ++i) {
        char command[512];
        snprintf(command, sizeof(command), "../optim/optim %s1.tmp %s2.tmp", input_files[i], input_files[i]);
        print_verbose("Optimizing...");
        printf("Executing: %s\n", command);
        if (system(command) != 0) {
            error("Optimization failed");
        }
    }
}

static void assemble_files() {
    for (size_t i = 0; i < input_idx; ++i) {
        char command[512];
        snprintf(command, sizeof(command), "../zas/zas -N -o%s.obj %s2.tmp", input_files[i], input_files[i]);
        print_verbose("Assembling...");
        printf("Executing: %s\n", command);
        if (system(command) != 0) {
            error("Assembly failed");
        }
    }
}

static void link_files() {
    char command[1024] = "../link/link -Z -Ptext=0,data,bss -C100H -O%s.COM CRTCPM.OBJ";
    for (size_t i = 0; i < input_idx; ++i) {
        strcat(command, " ");
        strcat(command, input_files[i]);
        strcat(command, ".obj");
    }
    for (size_t i = 0; i < lib_idx; ++i) {
        strcat(command, " ");
        strcat(command, libraries[i]);
    }
    strcat(command, " LIBC.LIB");

    print_verbose("Linking...");
    printf("Executing: %s\n", command);
    if (system(command) != 0) {
        error("Linking failed");
    }
}

// Função principal
int main(int argc, char **argv) {
    printf("C11 Build Tool Simulation\n");

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [options] files...\n", argv[0]);
        return EXIT_FAILURE;
    }

    parse_arguments(argc, argv);

    if (preprocess) {
        preprocess_files();
    }
    if (compile_flag) {
        compile_files();
    }
    if (generate_asm) {
        generate_assembly();
    }
    if (optimize) {
        optimize_files();
    }
    if (assemble) {
        assemble_files();
    }
    if (link_flag) {
        link_files();
    }

    return EXIT_SUCCESS;
}
