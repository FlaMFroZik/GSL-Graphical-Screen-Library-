#ifndef GSL_H
#define GSL_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	int gsl_init(const char* name, int width, int height);
	void gsl_shutdown(void);
	void gsl_fill(int color);
	void gsl_draw_text(int x, int y, const char* text, int color);
	int gsl_handle_events(void);

#ifdef __cplusplus
}
#endif

#endif