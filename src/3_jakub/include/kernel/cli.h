#ifndef KERNEL_CLI_H
#define KERNEL_CLI_H

#ifdef __cplusplus
extern "C" {
#endif

void cli_print_prompt(void);
void cli_submit_line(const char *line);
void cli_handle_escape(void);

#ifdef __cplusplus
}
#endif

#endif
