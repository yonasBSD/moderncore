#include <unistd.h>

#include "WaylandDataSource.hpp"
#include "util/Invoke.hpp"
#include "util/Panic.hpp"

WaylandDataSource::WaylandDataSource( wl_data_device_manager* manager, wl_data_device* device, const char** mime, size_t count, uint32_t serial )
    : m_source( wl_data_device_manager_create_data_source( manager ) )
{
    CheckPanic( mime && count > 0, "No mime types!" );

    static constexpr wl_data_source_listener listener = {
        .target = Method( SourceTarget ),
        .send = Method( SourceSend ),
        .cancelled = Method( SourceCancelled ),
        .dnd_drop_performed = Method( SourceDndDropPerformed ),
        .dnd_finished = Method( SourceDndFinished ),
        .action = Method( SourceAction )
    };

    wl_data_source_add_listener( m_source, &listener, this );

    for( size_t i=0; i<count; i++ ) wl_data_source_offer( m_source, mime[i] );
    wl_data_device_set_selection( device, m_source, serial );
}

WaylandDataSource::~WaylandDataSource()
{
    wl_data_source_destroy( m_source );
}

void WaylandDataSource::SetListener( const Listener* listener, void* ptr )
{
    m_listener = listener;
    m_listenerPtr = ptr;
}

void WaylandDataSource::SourceTarget( wl_data_source* source, const char* mimeType )
{
}

void WaylandDataSource::SourceSend( wl_data_source* source, const char* mimeType, int32_t fd )
{
    Invoke( OnSend, mimeType, fd );
    close( fd );
}

void WaylandDataSource::SourceCancelled( wl_data_source* source )
{
    Invoke( OnCancelled );
}

void WaylandDataSource::SourceDndDropPerformed( wl_data_source* source )
{
}

void WaylandDataSource::SourceDndFinished( wl_data_source* source )
{
}

void WaylandDataSource::SourceAction( wl_data_source* source, uint32_t dndAction )
{
}
