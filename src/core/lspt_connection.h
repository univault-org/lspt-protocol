   #pragma once

   class LSPTConnection {
   public:
       LSPTConnection(): connected(false) {};
       bool isConnected() const { return connected; }
   private:
       bool connected;
   };