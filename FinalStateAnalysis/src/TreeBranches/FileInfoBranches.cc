#include "JakobsVBSProcessor.h"


void JakobsVBSProcessor::SetFileInfoBranches( FileInfo &file_info ) {
	// File-specific

	_datatrain->Branch( "xsection" ,	&file_info.xsection,	"xsection/F" );
}
