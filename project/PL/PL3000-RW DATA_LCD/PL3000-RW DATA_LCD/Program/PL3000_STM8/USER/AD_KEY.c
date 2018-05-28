void AD_KEY_Fun();
{
  uint16_t touch_sw_key_value;
  touch_sw_key_value = ADC_Read_Channel_Fun(ADC_TOUCH_SW_CHANNEL);
  if(touch_sw_key_value >= TOUCH_SW_RELEASE)
  {
    virtual_IDR = KEY_FLASH_PIN | KEY_TOUCH_PIN;  /* not push */
  }else{
    if(touch_sw_key_value < FLASH_SW_PUSH)
    {
      virtual_IDR = KEY_TOUCH_PIN;  /* flash sw push */
    }else{
      virtual_IDR = KEY_FLASH_PIN;  /* touch  push */
    }
  }

  Flash_Key_Readdata = virtual_IDR ^ 0xff;
  Flash_Key_Tirg = Flash_Key_Readdata & (Flash_Key_Readdata ^ Flash_Key_Cont);
  Flash_Key_Cont = Flash_Key_Readdata;

  if((Flash_Key_Cont & KEY_TOUCH_PIN))
  {
    printf("touch push,value is %04x\r\n",touch_sw_key_value);
  }

  if((Flash_Key_Cont & KEY_FLASH_PIN))
  {
    printf("falsh push,value is %04x\r\n",touch_sw_key_value);
  }

  if((!(Flash_Key_Cont & KEY_TOUCH_PIN)) && (!(Flash_Key_Cont & KEY_FLASH_PIN)))
  {
    printf("release,value is %04x\r\n",touch_sw_key_value);
  }
}
