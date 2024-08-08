int main(int argc, const char **argv, const char **envp)
{
    int lock_status;
    char s[128];
    pid_t pid;
    int ptrace_return;

    pid = fork();
    memset(s, 0, sizeof(s));
    lock_status = 0;
    if (pid)
    {
        do
        {
            wait(&lock_status);
            if ((lock_status & 127) == 0 || ((char)((lock_status & 127) + 1) >> 1 > 0))
            {
                puts("child is exiting...");
                return 0;
            }
                ptrace_return = ptrace(PTRACE_PEEKUSER, pid, 44, 0);
        }
        while (ptrace_return != 11);
        puts("no exec() for you");
        kill(pid, 9);
    }
    else
    {
        prctl(1, 1);
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        puts("Give me some shellcode, k");
        gets(s);
    }
    return 0;
}