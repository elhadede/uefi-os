#include <efi.h>
#include <efilib.h>
#include <efiprot.h>



EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
 
  	int status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
  	if(EFI_ERROR(status))
    	Print(L"Unable to locate GOP");
     EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
  UINTN SizeOfInfo, numModes, nativeMode;
 
  status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
  // this is needed to get the current video mode
  if (status == EFI_NOT_STARTED)
    status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
  if(EFI_ERROR(status)) {
    Print(L"Unable to get native mode");
  } else {
    nativeMode = gop->Mode->Mode;
    numModes = gop->Mode->MaxMode;
  }
  int mode = 0;
  if(EFI_ERROR(status)) {
    Print(L"Unable to set mode %03d", mode);
  } else {
    // get framebuffer
    Print(L"Framebuffer address %x size %d, width %d height %d pixelsperline %d",
      gop->Mode->FrameBufferBase,
      gop->Mode->FrameBufferSize,
      gop->Mode->Info->HorizontalResolution,
      gop->Mode->Info->VerticalResolution,
      gop->Mode->Info->PixelsPerScanLine
    );
    
  }
  int pitch = 4 * gop->Mode->Info->PixelsPerScanLine;
  uint32_t pixel = 0x74;
  for(int x=0;x < gop->Mode->Info->HorizontalResolution;x++){
  	for(int y=0;y < gop->Mode->Info->HorizontalResolution;y++){
  		*((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * y + 4 * x)) = pixel;
  	}
  }
  
  	Print(L"hello world");
    for(;;) __asm__("hlt");
}
