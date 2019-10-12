#ifdef __cplusplus
extern "C"
{
#endif
	__declspec(dllimport) int mainAsLib(int argc, char **argv);
	__declspec(dllimport) extern char* outputBuffer;
	__declspec(dllimport) extern char* outputBufferStart;
	__declspec(dllimport) extern char* inputBuffer;
	__declspec(dllimport) extern char* inputBufferStart;
	/*__declspec(dllimport) extern int noFile;*/
#ifdef __cplusplus
}
#endif