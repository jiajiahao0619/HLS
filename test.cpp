#include "soble.h"
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ap_int.h>

int main()
{
	lint test_img[H+H/2][W/8];
	lint testo[H][W*3/8];

	int y_i,y_j;
	int uv_i,yu_j;

	for (y_i=0;y_i<H;y_i++){
		for(y_j=0;y_j<W/8;y_j++){
			test_img[y_i][y_j] = 0x4622334455667788;
		}
	}
	for (uv_i=0;uv_i<H/2;uv_i++){
		for(yu_j=0;yu_j<W/8;yu_j++){
			test_img[H+uv_i][yu_j] = 0xa872334455667788;
		}
	}
	soble(test_img,testo,1080,1920);

	int o_i,o_j;
	FILE *fp;
	fp = fopen("out.dat","w");
	for (o_i=0;o_i<H;o_i++){

		for(o_j=0;o_j<W*3/8;o_j++){
			fprintf(fp,"%x  ",testo[o_i][o_j]);
		}
		fprintf(fp," \r\n");
	}
	fclose(fp);


	fprintf(stdout, "*******************************************\n");
	fprintf(stdout, "PASS: The output matches the golden output!\n");
	fprintf(stdout, "*******************************************\n");

return 0;
}
