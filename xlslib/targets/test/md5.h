/*
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.  This code was
 * written by Colin Plumb in 1993, no copyright is claimed.
 * This code is in the public domain; do with it what you wish.
 *
 * Equivalent code is available from RSA Data Security, Inc.
 * This code has been tested against that, and is equivalent,
 * except that you don't need to include two pages of legalese
 * with every copy.
 *
 * To compute the message digest of a chunk of bytes, declare an
 * MD5Context structure, pass it to MD5Init, call MD5Update as
 * needed on buffers full of bytes, and then call MD5Final, which
 * will fill a supplied 16-byte array with the digest.
 */

/*
Code adapted from citadel (www.citadel.org ???)
*/

#ifndef MD5_H
#define MD5_H

#include "common/xlsys.h"
#include "common/systype.h"

#if defined(__cplusplus)
extern "C" 
{
#endif



struct MD5Context 
{
	unsigned32_t buf[4];
	unsigned32_t bits[2];
	unsigned32_t inp[16];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, unsigned char const *buf, size_t len);
void MD5Final(unsigned char digest[16], struct MD5Context *context);
void MD5Transform(unsigned32_t buf[4], unsigned32_t const inp[16]);

#define MD5_DIGEST_LEN		16
#define MD5_HEXSTRING_SIZE	(2*MD5_DIGEST_LEN+1)


/**
Load the binary file specified by the path @a filepath into the dynamically allocated 
buffer @a *buf_ref.

When called, the caller must pass a reference to a variable where the buffer pointer (@a buf_ref) will
be stored, plus a reference to a variable where the number of bytes loaded (@a buflen_ref) will
be stored.

@return 0 on success (and @a *buf_ref and @a *buflen_ref will have been set to point at the buffer
containing the file content) or a non-zero number on error (while @a *buf_ref and @a *buflen_ref will 
have been set to NULL and 0).

@note
Make sure to call @c free() to release the dynamically allocated buffer, once you're done perusing the
loaded data.
*/
int load_file(unsigned8_t **buf_ref, size_t *buflen_ref, const char *filepath);
/**
Calculate the MD5 hash (printed in lower case hexadecimal) for the given file.

@a md5_checksum must point to a character buffer large enough to contain the
printed hash (@ref MD5_HEXSTRING_SIZE characters or more). @a md5_checksum_bufsize
must specify the actual character buffer size available for @a mk_md5_4_file().

@return 0 on success, non-zero on error.
*/
int mk_md5_4_file(char *md5_checksum, size_t md5_checksum_bufsize, const char *filepath);

/**
Check whether the given file has the given MD5 checksum (which is passed in printed 
lowercase text in the @a md5_checksum parameter).

@return 0 when the MD5 matches the file, otherwise a non-zero value is returned. The
value "File Not Found" indicates that a load/memory failure occurred, while other non-zero
return values are due to an MD5 mismatch.
*/
char * check_file(const char *filepath, const char *md5_checksum);



#if defined(__cplusplus)
}
#endif

#endif /* !MD5_H */
