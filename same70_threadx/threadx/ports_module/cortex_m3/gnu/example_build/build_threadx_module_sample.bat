arm-none-eabi-gcc -c -g -mcpu=cortex-m3 -fpie -fno-plt -mno-pic-data-is-text-relative -msingle-pic-base txm_module_preamble.s
arm-none-eabi-gcc -c -g -mcpu=cortex-m3 -fpie -fno-plt -mno-pic-data-is-text-relative -msingle-pic-base gcc_setup.S
arm-none-eabi-gcc -c -g -mcpu=cortex-m3 -fpie -fno-plt -mno-pic-data-is-text-relative -msingle-pic-base -I..\inc -I..\..\..\..\common\inc -I..\..\..\..\common_modules\inc sample_threadx_module.c
arm-none-eabi-ld -A cortex-m3 -T sample_threadx_module.ld txm_module_preamble.o gcc_setup.o sample_threadx_module.o -e _txm_module_thread_shell_entry txm.a -o sample_threadx_module.axf -M > sample_threadx_module.map 

