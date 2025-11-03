#pragma once

#include <wayland-client.h>

#include "util/NoCopy.hpp"

class WaylandSeat;

class WaylandDataSource
{
public:
    struct Listener
    {
        void (*OnSend)( void* ptr, const char* mimeType, int32_t fd );
        void (*OnCancelled)( void* ptr );
    };

    WaylandDataSource( WaylandSeat& seat, wl_data_device_manager* manager, wl_data_device* device, const char** mime, size_t count, uint32_t serial );
    ~WaylandDataSource();

    NoCopy( WaylandDataSource );

    void SetListener( const Listener* listener, void* ptr );

private:
    void SourceTarget( wl_data_source* source, const char* mimeType );
    void SourceSend( wl_data_source* source, const char* mimeType, int32_t fd );
    void SourceCancelled( wl_data_source* source );
    void SourceDndDropPerformed( wl_data_source* source );
    void SourceDndFinished( wl_data_source* source );
    void SourceAction( wl_data_source* source, uint32_t dndAction );

    WaylandSeat& m_seat;
    wl_data_source* m_source;

    const Listener* m_listener = nullptr;
    void* m_listenerPtr;
};
