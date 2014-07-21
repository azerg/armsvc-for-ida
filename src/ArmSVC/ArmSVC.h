#ifdef PBCL_PROCLOC_EXPORTS
#define PBCL_PROCLOC_API __declspec(dllexport)
#else
#define PBCL_PROCLOC_API __declspec(dllimport)
#endif

extern PBCL_PROCLOC_API plugin_t PLUGIN;

int __stdcall IDAP_init();
void __stdcall IDAP_term();
void __stdcall IDAP_run(int arg);