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

    uint8 local_ou[W*3/8];
    
    uint16 hloop,wloop;
    uint8 i;
    uint8 y_8[8],u_8[8],v_8[8];
#pragma HLS ARRAY_PARTITION variable=v_8 complete dim=1
#pragma HLS ARRAY_PARTITION variable=u_8 complete dim=1
#pragma HLS ARRAY_PARTITION variable=y_8 complete dim=1
    uint16 y_16,u_16,v_16;
    uint16 yo_16,uo_16,vo_16;
    
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
            u_num = wloop;
            v_num = wloop;
            y = local_iny[y_num];
            u = local_inuv[u_num];
            v = local_inuv[v_num];
            
            third:
        	for( i=0;i<8;i++){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=8
                y_8[i] = uint8(y>>i*8);
                u_8[i] = uint8(u>>(i/2)*16);
                v_8[i] = uint8(v>>(2*(i/2)+1)*8);
                y_16 = uint16(y_8[i]);
                u_16 = uint16(u_8[i]);
                v_16 = uint16(v_8[i]);


                yo_16 = (y_16 + (u_16 - 128) + ((104*(u_16 - 128))/256));
                uo_16 = (y_16 - (89*(v_16 - 128)/256) - ((183*(u_16 - 128))/256));
                vo_16 = (y_16 + (v_16 - 128) + ((199*(v_16 - 128))/256));

                local_ou[wloop*24+i*3]=uint8(yo_16);
                local_ou[wloop*24+i*3+1]=uint8(uo_16);
                local_ou[wloop*24+i*3+2]=uint8(vo_16);
                //local_ou[wloop*3+0]=y_8[i]+1.402(u_8[i] -128);
                //local_ou[wloop*3+1]=y_8[i]-0.334*(u_8[i]-128)-0.714*(v_8[i]-128);
                //local_ou[wloop*3+2]=y_8[i]+1.772*(v_8[i]-128);


                if (i==7){
                    out_img[hloop][wloop] =((lint)((uint8)local_ou[wloop/8+0]))|(((lint)((uint8)local_ou[wloop/8+1]))<<8)|(((lint)((uint8)local_ou[wloop/8+2]))<<16)|(((lint)((uint8)local_ou[wloop/8+3]))<<24)|(((lint)((uint8)local_ou[wloop/8+4]))<<32)|(((lint)((uint8)local_ou[wloop/8+5]))<<40)|(((lint)((uint8)local_ou[wloop/8+6]))<<48)|(((lint)((uint8)local_ou[wloop/8+7]))<<56);
    
                }
        	}

        }    
    }

    
}
