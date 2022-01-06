#pragma once

#define ERROR(message) "[ERROR] "__FUNCTION__":"__LINE__" "##message
#define WARN(message) "[WARNING] "__FUNCTION__":"__LINE__" "##message
#define INFO(message) "[INFO] "##message