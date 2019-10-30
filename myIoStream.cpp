#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

// // My own implementation of IOStream
// class MyIOStream : public Assimp::IOStream
// {
//   friend class MyIOSystem;
// protected:
//   // Constructor protected for private usage by MyIOSystem
//   MyIOStream(void);
// public:
//   ~MyIOStream(void);
//   size_t Read( void* pvBuffer, size_t pSize, size_t pCount) { ... }
//   size_t Write( const void* pvBuffer, size_t pSize, size_t pCount) { ... }
//   aiReturn Seek( size_t pOffset, aiOrigin pOrigin) { ... }
//   size_t Tell() const { ... }
//   size_t FileSize() const { ... }
//   void Flush () { ... }
// };
// // Fisher Price - My First Filesystem
// class MyIOSystem : public Assimp::IOSystem
// {
//   MyIOSystem() { ... }
//   ~MyIOSystem() { ... }
//   // Check whether a specific file exists
//   bool Exists( const std::string& pFile) const {
//     .. 
//   }
//   // Get the path delimiter character we'd like to see
//   char GetOsSeparator() const { 
//     return '/'; 
//   }
//   // ... and finally a method to open a custom stream
//   IOStream* Open( const std::string& pFile, const std::string& pMode) {
//         return new MyIOStream( ... ); 
//   }
//   void Close( IOStream* pFile) { delete pFile; }
// };