#ifndef _TEE_INTERNAL_CORE_PROP_API_H_
#define _TEE_INTERNAL_CORE_PROP_API_H_

/** Core Framework Property API.
 *
 * Currently, bellow properties are supported:
 * - TA
 *	gpd.ta.appID					: UUID
 *	gpd.ta.singleInstance				: BOOL
 *	gpd.ta.multiSession				: BOOL
 *	gpd.ta.instanceKeepAlive			: BOOL
 *	gpd.ta.dataSize					: INTEGER
 *	gpd.ta.stackSize				: INTEGER
 * - Client
 *	gpd.client.identity				: IDENTITY
 * - TEE
 *	gpd.tee.apiversion				: STRING
 *	gpd.tee.description				: STRING
 *	gpd.tee.deviceID				: UUID
 *	gpd.tee.systemTime.protectionLevel		: INTEGER
 *	gpd.tee.TAPersistentTime.protectionLevel	: INTEGER
 *	gpd.tee.arith.maxBigIntSize			: INTEGER
 */

#include "tee_internal_core_common.h"

typedef enum {
	TEE_PROPSET_ID_CURRENT_TA		= 0xFFFFFFFF,
	TEE_PROPSET_ID_CURRENT_CLIENT		= 0xFFFFFFFE,
	TEE_PROPSET_ID_TEE_IMPLEMENTATION	= 0xFFFFFFFD,
	TEE_PROPSET_ID_ENVIROMENT_VARIABLE	= 0xFFFFFFF0,
	TEE_PROPSET_ID_MIN			= TEE_PROPSET_ID_ENVIROMENT_VARIABLE,
	TEE_PROPSET_ID_INVALID			= 0
} TEE_PropSet;

typedef struct TEE_PropEnumerator {
	TEE_PropSet	propset;
	uint32_t	index;
	const char*	name;
} TEE_PropEnumerator;

/** Handle on a property set or a property enumerator */
typedef struct TEE_PropEnumerator* TEE_PropSetHandle;

#define TEE_PROPSET_CURRENT_TA			\
	((TEE_PropSetHandle)TEE_PROPSET_ID_CURRENT_TA)
#define TEE_PROPSET_CURRENT_CLIENT		\
	((TEE_PropSetHandle)TEE_PROPSET_ID_CURRENT_CLIENT)
#define TEE_PROPSET_TEE_IMPLEMENTATION		\
	((TEE_PropSetHandle)TEE_PROPSET_ID_TEE_IMPLEMENTATION)

#define TEE_PROP_IS_PROPSET(handle)		\
	((TEE_PropSet)handle >= TEE_PROPSET_ID_MIN)
#define TEE_PROP_IS_ENUMERATOR(handle)		\
	((TEE_PropSet)handle < TEE_PROPSET_ID_MIN)

TEE_Result TEE_GetPropertyAsString(
	TEE_PropSetHandle	propsetOrEnumerator,
	const char*		name,
	char*			valueBuffer,
	size_t*			valueBufferLen);

TEE_Result TEE_GetPropertyAsBool(
	TEE_PropSetHandle	propsetOrEnumerator,
	const char*		name,
	bool*			value);

TEE_Result TEE_GetPropertyAsU32(
	TEE_PropSetHandle	propsetOrEnumerator,
	const char*		name,
	uint32_t*		value);

TEE_Result TEE_GetPropertyAsBinaryBlock(
	TEE_PropSetHandle	propsetOrEnumerator,
	const char*		name,
	void*			valueBuffer,
	size_t*			valueBufferLen);

TEE_Result TEE_GetPropertyAsUUID(
	TEE_PropSetHandle	propsetOrEnumerator,
	const char*		name,
	TEE_UUID*		value);

TEE_Result TEE_GetPropertyAsIdentity(
	TEE_PropSetHandle	propsetOrEnumerator,
	const char*		name,
	TEE_Identity*		value);

/* Property Enumerator.
 *
 * Flow to use a Property Enumerator:
 * 1. TEE_AllocatePropertyEnumerator()
 * 2. TEE_ResetPropertyEnumerator()
 * 3. TEE_StartPropertyEnumerator()
 * 4. TEE_GetPropertyName() or TEE_GetPropertyAsXXX()
 * 5. TEE_GetNextProperty()
 * 6. goto 4
 * 7. TEE_FreePropertyEnumerator() if no more actions
 */

TEE_Result TEE_AllocatePropertyEnumerator(
	TEE_PropSetHandle*	enumerator);

void TEE_FreePropertyEnumerator(
	TEE_PropSetHandle	enumerator);

void TEE_StartPropertyEnumerator(
	TEE_PropSetHandle	enumerator,
	TEE_PropSetHandle	propSet);

void TEE_ResetPropertyEnumerator(
	TEE_PropSetHandle	enumerator);

TEE_Result TEE_GetPropertyName(
	TEE_PropSetHandle	enumerator,
	void*			nameBuffer,
	size_t*			nameBufferLen);

TEE_Result TEE_GetNextProperty(
	TEE_PropSetHandle	enumerator);

#endif /* _TEE_INTERNAL_CORE_PROP_API_H_ */