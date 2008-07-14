///////////////////////////////////////////////////////////////////////////////
// Name:        tests/net/ipc.cpp
// Purpose:     IPC classes unit tests
// Author:      Vadim Zeitlin
// RCS-ID:      $Id$
// Copyright:   (c) 2008 Vadim Zeitlin
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
// and "wx/cppunit.h"
#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// this test needs threads as it runs the test server in a secondary thread
#if wxUSE_THREADS

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/app.h"
#endif

#include "wx/ipc.h"
#include "wx/thread.h"

namespace
{

const char *IPC_TEST_PORT = "4242";
const char *IPC_TEST_TOPIC = "IPC TEST";

} // anonymous namespace

// ----------------------------------------------------------------------------
// test connection class used by IPCTestServer
// ----------------------------------------------------------------------------

class IPCTestConnection : public wxConnection
{
public:
    IPCTestConnection() { }

    virtual bool OnExec(const wxString& topic, const wxString& data)
    {
        if ( topic != IPC_TEST_TOPIC )
            return false;

        return data == "Date";
    }

private:
    DECLARE_NO_COPY_CLASS(IPCTestConnection)
};

// ----------------------------------------------------------------------------
// event dispatching thread class
// ----------------------------------------------------------------------------

class EventThread : public wxThread
{
public:
    EventThread()
        : wxThread(wxTHREAD_JOINABLE)
    {
        Create();
        Run();
    }

protected:
    virtual void *Entry()
    {
        wxTheApp->MainLoop();

        return NULL;
    }

    DECLARE_NO_COPY_CLASS(EventThread)
};

// ----------------------------------------------------------------------------
// test server class
// ----------------------------------------------------------------------------

class IPCTestServer : public wxServer
{
public:
    IPCTestServer()
    {
        m_conn = NULL;

        // we must call this from the main thread
        wxSocketBase::Initialize();

        // we need event dispatching to work for IPC server to work
        m_thread = new EventThread;

        Create(IPC_TEST_PORT);
    }

    virtual ~IPCTestServer()
    {
        wxTheApp->ExitMainLoop();

        m_thread->Wait();
        delete m_thread;

        delete m_conn;

        wxSocketBase::Shutdown();
    }

    virtual wxConnectionBase *OnAcceptConnection(const wxString& topic)
    {
        if ( topic != IPC_TEST_TOPIC )
            return NULL;

        m_conn = new IPCTestConnection;
        return m_conn;
    }

private:
    EventThread *m_thread;
    IPCTestConnection *m_conn;

    DECLARE_NO_COPY_CLASS(IPCTestServer)
};

static IPCTestServer *gs_server = NULL;

// ----------------------------------------------------------------------------
// test client class
// ----------------------------------------------------------------------------

class IPCTestClient : public wxClient
{
public:
    IPCTestClient()
    {
        m_conn = NULL;
    }

    virtual ~IPCTestClient()
    {
        Disconnect();
    }

    bool
    Connect(const wxString& host, const wxString& service, const wxString& topic)
    {
        m_conn = MakeConnection(host, service, topic);

        return m_conn != NULL;
    }

    void Disconnect()
    {
        if ( m_conn )
        {
            delete m_conn;
            m_conn = NULL;
        }
    }

    wxConnectionBase& GetConn() const
    {
        CPPUNIT_ASSERT( m_conn );

        return *m_conn;
    }

private:
    wxConnectionBase *m_conn;

    DECLARE_NO_COPY_CLASS(IPCTestClient)
};

static IPCTestClient gs_client;

// ----------------------------------------------------------------------------
// the test code itself
// ----------------------------------------------------------------------------

class IPCTestCase : public CppUnit::TestCase
{
public:
    IPCTestCase() { }

private:
    CPPUNIT_TEST_SUITE( IPCTestCase );
        CPPUNIT_TEST( Connect );
        CPPUNIT_TEST( Execute );
        CPPUNIT_TEST( Disconnect );
    CPPUNIT_TEST_SUITE_END();

    void Connect();
    void Execute();
    void Disconnect();

    DECLARE_NO_COPY_CLASS(IPCTestCase)
};

// this test is not enabled by default because it requires an IPC server to run
//CPPUNIT_TEST_SUITE_REGISTRATION( IPCTestCase );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( IPCTestCase, "IPCTestCase" );

void IPCTestCase::Connect()
{
    gs_server = new IPCTestServer;

    // connecting to the wrong port should fail
    CPPUNIT_ASSERT( !gs_client.Connect("localhost", "2424", IPC_TEST_TOPIC) );

    // connecting using an unsupported topic should fail (unless the server
    // expects a ROT-13'd topic name...)
    CPPUNIT_ASSERT( !gs_client.Connect("localhost", IPC_TEST_PORT, "VCP GRFG") );

    // connecting to the right port on the right topic should succeed
    CPPUNIT_ASSERT( gs_client.Connect("localhost", IPC_TEST_PORT, IPC_TEST_TOPIC) );
}

void IPCTestCase::Execute()
{
    wxConnectionBase& conn = gs_client.GetConn();

    const wxString s("Date");
    CPPUNIT_ASSERT( conn.Execute(s) );
    CPPUNIT_ASSERT( conn.Execute(s.mb_str(), s.length() + 1) );

    char bytes[] = { 1, 2, 3 };
    CPPUNIT_ASSERT( conn.Execute(bytes, WXSIZEOF(bytes)) );
}

void IPCTestCase::Disconnect()
{
    gs_client.Disconnect();

    if ( gs_server )
    {
        delete gs_server;
        gs_server = NULL;
    }
}

#endif // wxUSE_THREADS
