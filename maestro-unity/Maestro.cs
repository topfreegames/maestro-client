using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Runtime.InteropServices;
using System;

public class MaestroClient : MonoBehaviour
{
	[UnmanagedFunctionPointer(CallingConvention.StdCall)]
	private delegate void DebugLog(string log);
	private static readonly DebugLog debugLog = DebugWrapper;
	private static readonly IntPtr functionPointer = Marshal.GetFunctionPointerForDelegate(debugLog);
	private static IntPtr maestroClient = IntPtr.Zero;

	private static void DebugWrapper(string log)
	{
		Debug.Log(log);
	}

	private static void Init(string maestroApiUrl, int pingInterval){
		Debug.Log ("Maestro client initializing!");
		LinkDebugInternal (functionPointer);
		maestroClient = ClientInternal (maestroApiUrl, pingInterval);
	}

	public static void Initialize(string maestroApiUrl){
		Init (maestroApiUrl, 30);
		if (InitializeInternal(maestroClient)){
			Debug.Log("Maestro client initialized successfully");
		} else {
			Debug.LogError ("Maestro client failed to initialize =/");
		}
	}

	public static void Initialize(string maestroApiUrl, string scheduler, string id){
		Init (maestroApiUrl, 30);
		if (InitializeInternal(maestroClient,scheduler,id)){
			Debug.Log("Maestro client initialized successfully");
		} else {
			Debug.LogError ("Maestro client failed to initialize =/");
		}
	}

	public static string GetAddress(){
		IntPtr ptr = GetAddressInternal (maestroClient);
		return Marshal.PtrToStringAuto (ptr);
	}

	public static int GetPingInterval() {
		return GetPingIntervalInternal (maestroClient);
	}

	public static void SetPingInterval(int pingInterval) {
		SetPingIntervalInternal (maestroClient, pingInterval);
	}

	public static bool Ping(){
		return PingInternal (maestroClient);
	}

	public static bool RoomOccupied() {
		return RoomOccupiedInternal (maestroClient);
	}

	public static bool RoomReady() {
		return RoomReadyInternal (maestroClient);
	}

	public static bool RoomTerminated() {
		return RoomTerminatedInternal (maestroClient);
	}

	public static bool RoomTerminating() {
		return RoomTerminatingInternal (maestroClient);
	}

	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_get_address")]
	private static extern IntPtr GetAddressInternal(IntPtr obj);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_get_ping_interval")]
	private static extern int GetPingIntervalInternal(IntPtr obj);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_initialize")]
	private static extern bool InitializeInternal(IntPtr obj);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_initialize_with_scheduler_and_id")]
	private static extern bool InitializeInternal(IntPtr obj, string scheduler, string id);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="set_debug")]
	private static extern void LinkDebugInternal([MarshalAs(UnmanagedType.FunctionPtr)]IntPtr debugCal);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_create_client_with_ping_interval")]
	private static extern IntPtr ClientInternal (string maestroApiUrl, int pingInterval);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_ping")]
	private static extern bool PingInternal (IntPtr obj);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_room_occupied")]
	private static extern bool RoomOccupiedInternal (IntPtr obj);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_room_ready")]
	private static extern bool RoomReadyInternal (IntPtr obj);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_room_terminated")]
	private static extern bool RoomTerminatedInternal (IntPtr obj);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_room_terminating")]
	private static extern bool RoomTerminatingInternal (IntPtr obj);
	[DllImport ("libmaestro", CallingConvention = CallingConvention.Cdecl, EntryPoint="internal_set_ping_interval")]
	private static extern void SetPingIntervalInternal(IntPtr obj, int pingInterval);
}
