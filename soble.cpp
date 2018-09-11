#include "soble.h"

void soble( lint in_img[H+H/2][W/8], lint out_img[H][W*3/8],coef setH, coef setW)
{
#pragma HLS DATAFLOW
#pragma HLS INTERFACE s_axilite port=setH
#pragma HLS INTERFACE s_axilite port=setW
#pragma HLS INTERFACE m_axi     port=out_img       offset=slave
#pragma HLS INTERFACE m_axi     port=in_img       offset=slave
#pragma HLS INTERFACE s_axilite port=return     bundle=control

	lint local_iny[W/8];
	lint local_inuv[W/8];

    uint16 y_num,u_num,v_num;
    lint y,u,v;

    uint8 local_ou[WIDTH/8*3];
    
    uint16 hloop,wloop;
    uint16 y_16,u_16,v_16;
    uint16 oy_16,ou_16,ov_16;
    uint8 i,j;
    uint8 y_8[8],u_8[8],v_8[8];
#pragma HLS ARRAY_PARTITION variable=v_8 complete dim=1
#pragma HLS ARRAY_PARTITION variable=u_8 complete dim=1
#pragma HLS ARRAY_PARTITION variable=y_8 complete dim=1
    
    first:
    for (hloop=0;hloop<H;hloop++){
    	memcpy(local_iny,in_img[hloop],W);
    	memcpy(local_inuv,in_img[hloop/2+H],W);

        uint16 step = hloop*H;
        second:
        for(wloop=0;wloop<(W/8);wloop++){
#pragma HLS PIPELINE II=1
            y_num = wloop;
            u_num = wloop;
            v_num = wloop;
            y = local_iny[y_num];
            u = local_inuv[u_num];
            v = local_inuv[v_num];

            third:
        	for( i=0;i<8;i++)
        	{
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=8

                y_8[i] = uint8(y>>i*8);
                u_8[i] = uint8(u>>(i/2)*16);
                v_8[i] = uint8(v>>(2*(i/2)+1)*8);
                y_16 = uint16(y_8[i]);
                u_16 = uint16(u_8[i]);
                v_16 = uint16(v_8[i]);


                oy_16 = (y_16 + (u_16 - 128) + ((104*(u_16 - 128))/256));
                ou_16 = (y_16 - (89*(v_16 - 128)>>8) - ((183*(u_16 - 128))/256));
                ov_16 = (y_16 + (v_16 - 128) + ((199*(v_16 - 128))>>8));


                local_ou[i*3] = uint8(oy_16);
                local_ou[i*3+1]=uint8(ou_16);
                local_ou[i*3+2]=uint8(ov_16);

        	}

                out_img[hloop][wloop*3] =(((lint)((uint8)local_ou[7]))<<56)|(((lint)((uint8)local_ou[6]))<<48)|(((lint)((uint8)local_ou[5]))<<40)|(((lint)((uint8)local_ou[4]))<<32)|(((lint)((uint8)local_ou[3]))<<24)|(((lint)((uint8)local_ou[2]))<<16)|(((lint)((uint8)local_ou[1]))<<8)|((lint)((uint8)local_ou[0]));
                out_img[hloop][wloop*3+1] =(((lint)((uint8)local_ou[15]))<<56)|(((lint)((uint8)local_ou[14]))<<48)|(((lint)((uint8)local_ou[13]))<<40)|(((lint)((uint8)local_ou[12]))<<32)|(((lint)((uint8)local_ou[11]))<<24)|(((lint)((uint8)local_ou[10]))<<16)|(((lint)((uint8)local_ou[9]))<<8)|((lint)((uint8)local_ou[8]));
                out_img[hloop][wloop*3+2] =(((lint)((uint8)local_ou[23]))<<56)|(((lint)((uint8)local_ou[22]))<<48)|(((lint)((uint8)local_ou[21]))<<40)|(((lint)((uint8)local_ou[20]))<<32)|(((lint)((uint8)local_ou[19]))<<24)|(((lint)((uint8)local_ou[18]))<<16)|(((lint)((uint8)local_ou[17]))<<8)|((lint)((uint8)local_ou[16]));

        }    
    }

    
}
