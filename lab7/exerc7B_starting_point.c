// Number of children
#define N 5

// Global variable that indicates how many children have terminated
int n = 0;

// Write signal handler for signal SIGCHLD here. 
// Signal handler increments n.

int main(void)
{
    // Install signal handler  
    // Open the file data.txt
    
    // Create N children that start to run a separate 
    // executable file child.exe. Pass the file descriptor,
    // the number of writes and the string that the child
    // program writes to the file.
   
    // Parent process waits for the children in the following loop.
    // Signal causes pause to return
    while (n < N) {
        pause();
        printf("Child term %d\n", n);
    }
    // Parent process displays the contents from the file
    return 0
}
