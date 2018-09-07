#include "soble.h"

void soble( lint in_img[H+H/2][W/8], lint out_img[W*3/8],coef setH, coef setW)
{
#pragma HLS DATAFLOW
#pragma HLS INTERFACE s_axilite port=setH
#pragma HLS INTERFACE s_axilite port=setW
#pragma HLS INTERFACE m_axi     port=out_img       offset=slave
#pragma HLS INTERFACE m_axi     port=in_img       offset=slave
#pragma HLS INTERFACE s_axilite port=return     bundle=control

	lint local_iny[W];
	lint local_inuv[W];

    uint16 y_num,u_num,v_num;
    uint16 y,u,v;

    uint8 local_ou[W*3];
    
    uint16 hloop,wloop;
    uint8 i;
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
#pragma HLS UNROLL factor=4
#pragma HLS PIPELINE II=1
            y_num = wloop;
            u_num = wloop/2;
            v_num = wloop/2+1;
            y = local_iny[y_num];
            u = local_inuv[u_num];
            v = local_inuv[v_num];
            
            third:
        	for( i=0;i<8;i++){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=8
                y_8[i] = uint8(y>>i*8);
                u_8[i] = uint8(u>>i*8);
                v_8[i] = uint8(v>>i*8);
                local_ou[wloop*3+0]=y_8[i]+(u_8[i] -128)+((104*(u_8[i] -128))>>8);
                local_ou[wloop*3+1]=y_8[i]+(89*(v_8[i]>>8))-((183*(u_8[i] -128))>>8);
                local_ou[wloop*3+2]=y_8[i]+(v_8[i]-128)+((119*(v_8[i]-128))>>8);
    
                if (wloop%8==0){
                    out_img[wloop] =((lint)((uint8)local_ou[wloop/8+0]))|(((lint)((uint8)local_ou[wloop/8+1]))<<8)|(((lint)((uint8)local_ou[wloop/8+2]))<<16)|(((lint)((uint8)local_ou[wloop/8+3]))<<24)|(((lint)((uint8)local_ou[wloop/8+4]))<<32)|(((lint)((uint8)local_ou[wloop/8+5]))<<40)|(((lint)((uint8)local_ou[wloop/8+6]))<<48)|(((lint)((uint8)local_ou[wloop/8+7]))<<56);
    
                }
        	}

        }    
    }

    
}
