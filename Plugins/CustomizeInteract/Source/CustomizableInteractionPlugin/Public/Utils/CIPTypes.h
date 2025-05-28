#pragma once

#define CIP_PRINT_FILE (FString(FPaths::GetCleanFilename(TEXT(__FILE__))))
#define CIP_PRINT_FUNC (FString(__FUNCTION__))
#define CIP_PRINT_LINE (FString::FromInt(__LINE__))
#define CIP_LOGS_LINE (CIP_PRINT_FUNC + TEXT(" [") + CIP_PRINT_FILE + TEXT(":") + CIP_PRINT_LINE + TEXT("]"))