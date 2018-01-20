typedef char String<>;

program COMMANDPROG {
    version COMMANDVERS {
        String EXECCOMMAND(string) = 1;
    } = 1;
} = 0x20000008;
