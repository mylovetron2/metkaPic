#include <16F887.h>

#device ADC=10
#fuses HS,NOWDT,NOPROTECT,NOLVP 
#use delay(clock=20000000)
//#use rs232(baud=128000,parity=e, xmit=PIN_C6, rcv=PIN_C7)
#use rs232(baud=115200,parity=e, xmit=PIN_C6, rcv=PIN_C7)


#bit TR1EN = 0x10.0

#byte porta = 0x05
#byte portb = 0x06
#byte portc = 0x07
#byte portd = 0x08

#bit hpeak    = portb.0          
#bit mark     = portb.1          
#bit start    = portb.2          
#bit page     = portb.3          
#bit clear    = portb.4          
#bit gaussing = portb.5          
#bit erasepro = portb.6          
#bit eraseall = portb.7          

#bit dir        = portc.0          
#bit pwm1       = portc.1          
//#bit dirmark    = portc.2          
#bit length     = portc.3          
#bit dq         = portc.4          
#bit get        = portc.5    

#bit ckq        = porta.4

#bit portd3     = portd.3

#define LCD_RS_PIN      PIN_D0                                   
#define LCD_RW_PIN      PIN_D1  
#define LCD_ENABLE_PIN  PIN_D2
#define LCD_DATA4       PIN_D4                                   
#define LCD_DATA5       PIN_D5                                   
#define LCD_DATA6       PIN_D6                                    
#define LCD_DATA7       PIN_D7 


     
#include <lcd.c>


unsigned int8 ipage=0,npage=0,run1,run2=0,run3=0;

unsigned int8 donvi,chuc,tram,nghin,chucnghin,tramnghin,trieu,chuctrieu,tramtrieu,ty;

int16 atens=1000,btens=0;
int32 lspeed;
signed int32 depth=0,olddepth,sdepth=0,ddq=0,ddqold=0;
int16 coef_encoder=19690;   
signed int16 delta=0;
int16 mag,i,tens,n_but,wflash_count,tt1=0,kk=0;
int8 buffer_com[8],buf[6],buffer[20],data[48],speed_flag=0;
int8 com_count=0,ncom=0,dd,dmt_flag=0,nd=0,flag_com=0;
int16 speed_buf[12] ={0,0,0,0,0,0,0,0,0,0,0,0};

int1 but_flag=0,yes=1,no=0,flag_ext=0,flag_dir=0,flag_peek=0,dirmark=1;
int8 mark_base=0,mark_length=0,mark_100=0,mark_500=0,base_length=10;
float fdepth,fspeed,ftens,ft;
unsigned int16 adbuf[8];
//===============================================

#INT_RDA

void serial_isr()
{
  
   buffer_com[ncom] = fgetc();        //doc lien tiep 8 bytes
   ncom++;
   if(ncom == 8)  {
      ncom=0;              //tu PC luon truy xuat bang lenh 8 byte      
      switch(buffer_com[0])   {         
         case 0xdd:                                         //    gui PC 138 byte gia lap do sau            
                     sdepth = make32(buffer_com[1],buffer_com[2],buffer_com[3],buffer_com[4]);
                     flag_com = 1;
                     break;        
         case 0xde:  flag_com=2;
                     tt1=0;               //khi co truy xuat tu pc tt1 khong vuot qua 10s 
                     break; 
         case 0xcd:  coef_encoder = make32(buffer_com[1],buffer_com[2],buffer_com[3],buffer_com[4]);
                     flag_com = 3;
                     break;
         case 0xdf:  ddq = make32(buffer_com[1],buffer_com[2],buffer_com[3],buffer_com[4]);
                     mark_500    =  ddq/500;                         //buffer_com[4];
                     mark_100    =  (ddq%500)/100;                   //buffer_com[3];
                     mark_length =  ((ddq%500)%100)/base_length;     //buffer_com[2];
                     mark_base   =  ddq%base_length;                 //buffer_com[1];
                     break;
                     
         case 0xdc:  atens = make16(buffer_com[1],buffer_com[2]);
                     btens = make16(buffer_com[3],buffer_com[4]);
                     flag_com=4;
                     break;
                     
         case 0xda:  coef_encoder = make32(buffer_com[1],buffer_com[2],buffer_com[3],buffer_com[4]);
                     flag_com = 3;
                     break;         
                     
         case 0xdb:  ipage++;
                     if(ipage > 2) ipage = 0;
                     break;             
         default:    break; 
      } 
   }
        
        
  
      
  
    
   
   
}

//================================================   ngat ngoai
#INT_EXT 
void  ext_isr()
{
   flag_ext = 1;
   flag_peek = 1;
   disable_interrupts(INT_EXT );
   run2=0;
 //  if (dir == dirmark) {
   
   gaussing = no;             //danh dau met
   mark_base++;
   eraseall = no;             //bat bo xoa tong
   if (mark_base > base_length) {
      mark_base =0;
      mark_length++;
      erasepro = yes;         //tat bo xoa 
   }
   
 //  else if(mark_base==1) 
 //  erasepro = yes;         //tat bo xoa 
   else 
   erasepro = no;        //bat bo xoa
    
   if(mark_length*base_length >= 100) { 
         mark_100++; 
         mark_length=0;
   }
   
   if(mark_100 >=5){
      mark_500++; 
      mark_100=0;
   }
   
   if(mark_500 == 20){
      mark_500 = 0; 
      
   }
   
 /*  }
   else {
      erasepro = yes;         //tat bo xoa
      eraseall = yes;
      flag_dir=1;
  }
*/   
      
} 

#INT_RTCC
void rtc_isr()
{
   set_timer0(255);
   pwm1  =no;
   shift_left(buf,6,dq);
   pwm1  =yes;
   nd++;
}
// 100ms
//   do toc do 12*0.5s = m/phut

#INT_TIMER1
void timer1_isr()
{
   set_timer1(3035);             //100ms
   speed_flag++;
   tt1++;
   run2++;
   run3++;
   if(speed_flag >= 5){    //500ms
   
   for (i=11;i>0;i--)   speed_buf[i]= speed_buf[i-1];
   speed_buf[0] = abs(depth - olddepth); 
   olddepth = depth; 
   
   lspeed =0;
   for (i=0;i<12;i++)   lspeed += speed_buf[i];             //so xung/ phut
   speed_flag=0;
   
   }
   
   
}


//=======================================================  gui du lieu
void send_com(void)
{
   
for(i=0; i < 48; i++)    fputc(data[i]);  

fputc(0xaa);
fputc(0xaa);

}



//===================================================chuan bj hien thj so
void tach_so(int32 num)
{
int32 temp;

temp=num/1000000000;
ty=temp;

temp=num%1000000000;
temp/=100000000;
tramtrieu=temp;

temp=num%100000000;
temp/=10000000;
chuctrieu=temp;

temp=num%10000000;
temp/=1000000;
trieu=temp;

temp=num%1000000;
temp/=100000;
tramnghin=temp;

temp=num%100000;
temp/=10000;
chucnghin=temp;

temp=num%10000;
temp/=1000;
nghin=temp;

temp=num%1000;
temp/=100;
tram=temp;

temp=num%100;
temp/=10;
chuc=temp;

donvi=num%10;
}

void out_num_ty()
{
lcd_putc(ty+48);
lcd_putc(tramtrieu+48);
lcd_putc(chuctrieu+48);
lcd_putc(trieu+48);
lcd_putc(tramnghin+48);
lcd_putc(chucnghin+48);
lcd_putc(nghin+48);
lcd_putc(tram+48);
lcd_putc(chuc+48);
lcd_putc(donvi+48);
}
void write_num_ty(int32 num)
{
tach_so(num);
out_num_ty();
}
//===================================================chuan bj hien thj so
void tach_so_8(int8 num)
{
int8 temp;

temp/=100;
tram=temp;

temp=num%100;
temp/=10;
chuc=temp;

donvi=num%10;
}

//=============================================================
void out_num_8()
{

if(tram!=0) lcd_putc(tram+48);

if((chuc!=0)|(tram!=0)) lcd_putc(chuc+48);
      
lcd_putc(donvi+48);

}
void write_num_8(int8 num)
{
tach_so_8(num);
out_num_8();
}

//=============================================================
void out_num(int8 dot)
{
if(trieu!=0)
{lcd_putc(trieu+48);}

if((tramnghin!=0)|(trieu!=0))
{lcd_putc(tramnghin+48);}

if((tramnghin!=0)|(chucnghin!=0)|(trieu!=0))
{lcd_putc(chucnghin+48);}

if((tramnghin!=0)|(chucnghin!=0)|(trieu!=0)|(nghin!=0)|(dot==3))
 {lcd_putc(nghin+48);
  if(dot==3)
      {lcd_putc('.');}
 }
if((tramnghin!=0)|(chucnghin!=0)|(trieu!=0)|(nghin!=0)|(tram!=0)|(dot==2)) 
{ lcd_putc(tram+48);
   if(dot==2)
      {lcd_putc('.');}
} 
if((nghin!=0)|(tram!=0)|(chuc!=0)|(dot==1)) 
{
lcd_putc(chuc+48);
      if(dot==1)
      {lcd_putc('.');}
}
lcd_putc(donvi+48);
}


//==================================================== write num
 
void write_num(int32 num,int8 dot)
{
tach_so(num);
out_num(dot);
}
//==================================================== 
void backup_depth (void) 
{
   write_EEPROM(3,sdepth);
   write_EEPROM(2,sdepth>>8);
   write_EEPROM(1,sdepth>>16);
   write_EEPROM(0,sdepth>>24);
    
}
//==================================================== 
void backup_encoder (void) 
{
   write_EEPROM(7,coef_encoder);
   write_EEPROM(6,coef_encoder>>8);
   write_EEPROM(5,coef_encoder>>16);
   write_EEPROM(4,coef_encoder>>24);
  
}
void backup_tens (void) 
{
   write_EEPROM(8,atens);
   write_EEPROM(9,atens>>8);
   
   write_EEPROM(10,btens);
   write_EEPROM(11,btens>>8);
  
}
//==================================================== 
void update_flash (void) 
{
   backup_depth();
   backup_encoder();
   backup_tens();
   
}
//===================================================
void read_flash (void) 
{
   
   for (i=0;i<12;i++) 
   {
      buffer[i] = read_eeprom(i);
   }     
   sdepth  =   make32(buffer[0],buffer[1],buffer[2],buffer[3]);
   coef_encoder = make32(buffer[4],buffer[5],buffer[6],buffer[7]);
   atens =   make16(buffer[9],buffer[8]); 
   btens =   make16(buffer[11],buffer[10]);
}

void hienthi(int8 npage)
{
if (npage==0) 
   {
      
      
      lcd_gotoxy(1,1);
      lcd_putc("DEPTH:");
      lcd_putc("            ");
      lcd_gotoxy(7,1);
      if(sdepth < 0)  lcd_putc('-');
      fdepth = abs(sdepth);
      fdepth *= 100;        
      fdepth /= coef_encoder;  //do sau theo dm
      write_num(fdepth,1);
      lcd_putc("m");
      lcd_gotoxy(17,1);
      if(dir) lcd_putc("UP");
      else lcd_putc("DN");
      
      //-------------------------------
    
      lcd_gotoxy(1,2);
      lcd_putc("SPEED:" );
      lcd_putc("             ");     
      lcd_gotoxy(7,2);
      fspeed = lspeed;
      fspeed *=1000;
      fspeed /= coef_encoder;
      write_num(fspeed,1);
      lcd_putc("m/min");
      lcd_gotoxy(17,2);
      if(dirmark) lcd_putc("UP");
      else lcd_putc("DN");
      
      //---------------------------------
      lcd_gotoxy(21,1);
      lcd_putc("TENS :");
      lcd_putc("            ");
      lcd_gotoxy(27,1);
      write_num(ftens,0);
      lcd_putc("kg");
      lcd_gotoxy(37,1);
      write_num_8(base_length);
      lcd_putc(' ');
            
      //----------------------------------                  
      if(mag>532)  lcd_putc('*');
      else lcd_putc(' ');
      
      //-------------------  
       
      lcd_gotoxy(21,2);                    // gia tri do sau theo dau met
      lcd_putc("MARK :");
      lcd_putc("              ");
      lcd_gotoxy(27,2);
      write_num_8(mark_500);
      lcd_putc(":");
      write_num_8(mark_100);
      lcd_putc(":");
      write_num_8(mark_length);
      lcd_putc(":");
      write_num_8(mark_base);
      
      lcd_putc(" ");
      if(ft<1000) write_num(ft,1);
      lcd_putc("m");
          
   }
   else if(npage==1)
   //-------------------------------------- hien so xung va he so encoder
   {
      lcd_gotoxy(1,1); 
      lcd_putc("Raw :");
      write_num_ty(depth);
      
      
      lcd_gotoxy(1,2); 
      lcd_putc("Coef:");
      write_num(coef_encoder,0);
      lcd_putc("      ");                   //he so encoder
             
      lcd_gotoxy(21,1);                    // suc cang
      lcd_putc("ATens:");
      write_num(atens,0);
      lcd_putc("      ");
   
      lcd_gotoxy(21,2);                   // dau met
      lcd_putc("BTens:");  
      write_num(btens,0);
      lcd_putc("      ");
                          
   }
   else if(npage==2)
   {
      lcd_gotoxy(1,1);                   
      lcd_putc("Ain0:");
      lcd_putc("     ");
      lcd_gotoxy(6,1);
      write_num(adbuf[0],0); 
            
      lcd_gotoxy(11,1);
      lcd_putc("Ain1:");
      lcd_putc("     ");
      lcd_gotoxy(16,1);
      write_num(adbuf[1],0); 
      
      lcd_gotoxy(1,2);                   
      lcd_putc("Ain2:");
      lcd_putc("     ");
      lcd_gotoxy(6,2);
      write_num(adbuf[2],0); 
            
      lcd_gotoxy(11,2);
      lcd_putc("Ain3:");
      lcd_putc("     ");
      lcd_gotoxy(16,2);
      write_num(adbuf[3],0);
      
      lcd_gotoxy(21,1);                   
      lcd_putc("Ain4:");
      lcd_putc("     ");
      lcd_gotoxy(26,1);
      write_num(adbuf[4],0); 
            
      lcd_gotoxy(31,1);
      lcd_putc("Ain5:");
      lcd_putc("     ");
      lcd_gotoxy(36,1);
      write_num(adbuf[5],0); 
      
      lcd_gotoxy(21,2);                   //kenh adc 4- dong ac
      lcd_putc("Ain6:");
      lcd_putc("     ");
      lcd_gotoxy(26,2);
      write_num(adbuf[6],0); 
            
      lcd_gotoxy(31,2);
      lcd_putc("Ain7:");
      lcd_putc("     ");
      lcd_gotoxy(36,2);
      write_num(adbuf[7],0);
      
                  
   }
}

void update_data()
{
if(sdepth<0) data[0]=1;
else data[0]=0;
   //data[1-6] gia tri bcd cua depth data[0] bit dau
tach_so(fdepth);
  data[1] = donvi;
  data[2]= chuc;
  data[3]= tram;
  data[4]= nghin;
  data[5]= chucnghin;
  data[6]= tramnghin;

   //data[7-11] gia tri bcd cua tension (suc cang cap)
tach_so(ftens);   
  data[7] = donvi;
  data[8]= chuc;
  data[9]= tram;
  data[10]= nghin;
  data[11]= chucnghin;
   
     //data[12-15] gia tri bcd cua speed
tach_so(fspeed);      
  data[12]= donvi;
  data[13]= chuc;
  data[14]= tram;
  data[15]= nghin;  
   //gia tri do sau
  data[16] = sdepth;             //depth from pic16f877a
  data[17] = sdepth>>8;
  data[18] = sdepth>>16;
  data[19] = sdepth>>24;
 // ----------------------------goi gia tri goc  
 
 data[20] = adbuf[0];      //tens
 data[21] = adbuf[0]>>8;      
 
 data[22] = adbuf[1];            //mag
 data[23] = adbuf[1]>>8;
   
 data[24] = adbuf[2];            //res
 data[25] = adbuf[2]>>8;

 data[26] = adbuf[3];            //nvac
 data[27] = adbuf[3]>>8;

 data[28] = adbuf[4];            //niac
 data[29] = adbuf[4]>>8;

 data[30] = adbuf[5];
 data[31] = adbuf[5]>>8;
   
 data[32] = adbuf[6];         //nvdc
 data[33] = adbuf[6]>>8;
 
 data[34] = adbuf[7];         //nidc
 data[35] = adbuf[7]>>8;
   
 
 

 
 data[40] = delta;           //ltimer*100ms: thoi gian giua 2 lan truy xuat tu pc
 data[41] = delta>>8;
 
}

//===========================================================
void main(void)
{
  

set_tris_A(0xff);
set_tris_B(0x1f);     
set_tris_C(0x9d);
set_tris_D(0x00);


enable_interrupts(INT_EXT);
enable_interrupts(INT_RDA);         //ngat nhan du lieu
enable_interrupts(INT_RTCC );
enable_interrupts(INT_TIMER1 );
enable_interrupts(GLOBAL);

setup_adc(ADC_CLOCK_INTERNAL);
Setup_ADC_ports(ALL_ANALOG);
SETUP_TIMER_0(RTCC_EXT_H_TO_L);
SETUP_TIMER_1(T1_INTERNAL|T1_DIV_BY_8);                     //1.6us
SET_TIMER1(3035);      
SET_TIMER0(255);                       //100ms
TR1EN=1;

 
lcd_init();

//---------------------------------------
ipage=0;

gaussing=yes;
pwm1 = yes;
      
erasepro=yes;
eraseall=no;      //luon bat xoa tong
sdepth=0;
mark_base=0;
run1 = read_eeprom(17);      
   if(run1 == 2)
   {   read_flash();
   }
   else
   {
      
      sdepth=0;
   }
      
while(1)
{
if(adbuf[7] > 10) {
   mark_length = mark_length*base_length + mark_base;
   base_length = 25;
   mark_base = mark_length%base_length;
   mark_length /= base_length;
   
    
}
else {
   mark_length = mark_length*base_length + mark_base;
   base_length = 10;
   mark_base = mark_length%base_length;
   mark_length /= base_length;
   
}

if(adbuf[6] > 10) {
   
   dirmark = 1;
   
}
else {
   dirmark = 0;
}

//------------------------------------------------- doc ad suc cang
for(i=0;i<8;i++){   
   adbuf[i]=0;
}
for(kk=0;kk<50;kk++){
for(i=0;i<8;i++){
   set_adc_channel(i);                               
   delay_us(10);
   adbuf[i] +=read_adc();
}
}
for(i=0;i<8;i++){   
   adbuf[i]/=50;
}
tens=adbuf[0];
mag=adbuf[1];
      
ftens = tens;
ftens *= atens;
ftens /=1000;                     //3 so le
ftens += btens;

if(flag_com == 3)          //cap nhat he so encoder
{
   flag_com = 0;
   write_EEPROM(17,2);   //run1=2
   backup_encoder () ;
}  
else if(flag_com == 1)          //nhan du lieu update
{
   flag_com = 0;
   write_EEPROM(17,2);   //run1=2
   backup_depth () ;
}
else if(flag_com == 4)          //nhan he sou tension
{
   flag_com = 0;
   write_EEPROM(17,2);   //run1=2
   backup_tens () ;
} 
else if(flag_com == 2)
{
   flag_com = 0; 
   get=no;              //bat truy xuat pic12f675
   delay_ms(4);            //cho doc 6 byte
   get=yes;
    
   depth = make32(buf[3],buf[2],buf[1],buf[0]);    //do sau goc khong update duoc
   delta = make16(buf[5], buf[4]);
   sdepth = sdepth + delta;  
   
   send_com();
   nd=0;
   hienthi(ipage);
}
/*
if (dir != dirmark){
   erasepro = yes;      //neu khac huong danh dau met tat bo xoa lap trinh va xoa tong
   eraseall = yes;
}
 */
if (run3>=3)
{   
   run3=0;
   
   
  if (tt1>20)  {
   flag_com = 2 ;                //tu truy xuat 12f675
   
 }
 else hienthi(ipage);
}
  
  
 if(flag_peek) {
   flag_peek=0;
   
   get=no;              //bat truy xuat pic12f675
   
   delay_ms(4);            //cho doc 6 byte
   get=yes;
    
   depth = make32(buf[3],buf[2],buf[1],buf[0]);    //do sau goc khong update duoc
   delta = make16(buf[5], buf[4]);
   sdepth = sdepth + delta;
   ddq = abs(depth-ddqold);
   ddqold = depth;
   data[36] = depth;            //raw depth from pic12f675 co dau met dinh
   data[37] = depth>>8;
   data[38] = depth>>16;
   data[39] = depth>>24;
    nd=0;
   ft = ddq;
   ft *=100;
   ft /= coef_encoder;
 
   if(flag_dir){
      flag_dir =0;
       
      if(ft > 4){ 
         mark_base =0;
             
         mark_length--;
      
         
   
         if(mark_length == 255) { 
         mark_100--; 
         mark_length = 99/base_length;       // = 9 hoac = 3
         
         }
   
   
         if(mark_100 == 255){
            mark_500--; 
            mark_100 = 4;
         }
   
   
         if(mark_500 == 255){
            mark_500=19;
         }
      }
   }
   
 }
  
if((flag_ext==1)&&(run2>=5)) {
   flag_ext = 0;
   run2=0;
   clear_interrupt(INT_EXT);
   enable_interrupts(INT_EXT );
   
}
  
if(!gaussing){
      delay_ms(5);
      gaussing = yes;
}
 
//------------------------------------------------------
n_but++;
if(n_but>50)
{
   n_but=0;
   but_flag=0;
}
if(but_flag==0)
{
   if(!page)                  //chuyen trang
   {
      ipage++;
      if(ipage>2) ipage=0;
      
      lcd_gotoxy(1,1);                   
      lcd_putc("                                        ");
      lcd_gotoxy(1,2);                   
      lcd_putc("                                        ");
       n_but=0;     
      but_flag=1;
   }
      
         //---------------------------------------------- clear depth   
   else if(!clear)
   {
      sdepth=0;
      n_but=0;      
      but_flag=1;
      
   }  
   else if(!mark)
   {
      gaussing=no;
      
      delay_ms(2);
      
      gaussing=yes;
      n_but=0;      
      but_flag=1;
   }
   else if(!start)
   {
      
      mark_base=0;
      mark_length=0;
      mark_100=0;
      mark_500=0;
      ipage=0;
      gaussing=no;
      delay_ms(2);
      gaussing=yes;
      n_but=0;      
      but_flag=1;
      erasepro=yes;
      eraseall=no;      //luon bat xoa tong
      
   }
}  
     
  
  //==------------------------------------------ ghj du lieu vao flash
wflash_count++;
if (wflash_count>=500)                            //2.4s/ ghj 1 lan
{
    
     backup_depth(); 
     //update_flash();
  
  wflash_count=0;
  portd3 ^=yes;                  // kiem tra thoi gian cap nhat flash      
}  
update_data();
 //fputc(PC1,0xaa);
//----------------------------------------chuan bj du lieu gui cong com


 
 
   
//-----------------------------------------gui cong com



}
}

