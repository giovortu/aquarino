#include "tools.h"


int hexToInt(char c)
{
  if (c >= '0' && c <= '9')
  {
    return c - '0';
  }
  if (c >= 'A' && c <= 'F')
  {
    return c - 'A' + 10;
  }
  if (c >= 'a' && c <= 'f')
  {
    return c - 'a' + 10;
  }
  return -1;
}



String zeroPadding(String p_input, unsigned int p_tot_digits)
{
  while (p_input.length() < p_tot_digits)
  {
    p_input = "0" + p_input;
  }
  return p_input;
}


String getTime()
{
  time_t now = time(NULL);
  struct tm *info;
  info = localtime(&now);
  String sTime = String(info->tm_year + 1900) + "-" +
                 zeroPadding(String(info->tm_mon + 1), 2) + "-" +
                 zeroPadding(String(info->tm_mday), 2) + "T" +
                 zeroPadding(String(info->tm_hour), 2) + ":" +
                 zeroPadding(String(info->tm_min), 2) + ":" +
                 zeroPadding(String(info->tm_sec), 2) + "Z";

  return sTime;
}

