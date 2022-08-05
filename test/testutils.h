#ifndef testutils_h
#define testutils_h

int execute(const char* command, char* result);
void assert_margin(long actual, long expected, float margin);

#endif /* testutils_h */
