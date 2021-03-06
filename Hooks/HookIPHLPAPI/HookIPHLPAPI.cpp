#include "HookIPHLPAPI.h"

#include <ShlObj.h>

#include "LuaHookNG.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);
			DllReExport::DllReExport();
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

void DllReExport::DllReExport() {
	TCHAR sysPath[MAX_PATH];
	SHGetFolderPath(nullptr,
					CSIDL_SYSTEM,
					nullptr,
					SHGFP_TYPE_CURRENT,
					sysPath);
	wcscat_s(sysPath, L"\\IPHLPAPI.dll");
	hDLL = LoadLibrary(L"IPAPLHPI.dll");
	if (!hDLL)
		hDLL = LoadLibrary(sysPath);
	if (!hDLL)
		throw std::wstring(L"Failed to load underlying DLL.\n");

	LuaHookNG::LuaHook_NG();  // force DLL import

	REEXPORT(AddIPAddress);
	REEXPORT(AllocateAndGetInterfaceInfoFromStack);
	REEXPORT(AllocateAndGetIpAddrTableFromStack);
	REEXPORT(CancelIPChangeNotify);
	REEXPORT(CancelMibChangeNotify2);
	REEXPORT(CloseCompartment);
	REEXPORT(CloseGetIPPhysicalInterfaceForDestination);
	REEXPORT(ConvertCompartmentGuidToId);
	REEXPORT(ConvertCompartmentIdToGuid);
	REEXPORT(ConvertGuidToStringA);
	REEXPORT(ConvertGuidToStringW);
	REEXPORT(ConvertInterfaceAliasToLuid);
	REEXPORT(ConvertInterfaceGuidToLuid);
	REEXPORT(ConvertInterfaceIndexToLuid);
	REEXPORT(ConvertInterfaceLuidToAlias);
	REEXPORT(ConvertInterfaceLuidToGuid);
	REEXPORT(ConvertInterfaceLuidToIndex);
	REEXPORT(ConvertInterfaceLuidToNameA);
	REEXPORT(ConvertInterfaceLuidToNameW);
	REEXPORT(ConvertInterfaceNameToLuidA);
	REEXPORT(ConvertInterfaceNameToLuidW);
	REEXPORT(ConvertInterfacePhysicalAddressToLuid);
	REEXPORT(ConvertIpv4MaskToLength);
	REEXPORT(ConvertLengthToIpv4Mask);
	REEXPORT(ConvertRemoteInterfaceAliasToLuid);
	REEXPORT(ConvertRemoteInterfaceGuidToLuid);
	REEXPORT(ConvertRemoteInterfaceIndexToLuid);
	REEXPORT(ConvertRemoteInterfaceLuidToAlias);
	REEXPORT(ConvertRemoteInterfaceLuidToGuid);
	REEXPORT(ConvertRemoteInterfaceLuidToIndex);
	REEXPORT(ConvertStringToGuidA);
	REEXPORT(ConvertStringToGuidW);
	REEXPORT(ConvertStringToInterfacePhysicalAddress);
	REEXPORT(CreateAnycastIpAddressEntry);
	REEXPORT(CreateIpForwardEntry);
	REEXPORT(CreateIpForwardEntry2);
	REEXPORT(CreateIpNetEntry);
	REEXPORT(CreateIpNetEntry2);
	REEXPORT(CreatePersistentTcpPortReservation);
	REEXPORT(CreatePersistentUdpPortReservation);
	REEXPORT(CreateProxyArpEntry);
	REEXPORT(CreateSortedAddressPairs);
	REEXPORT(CreateUnicastIpAddressEntry);
	REEXPORT(DeleteAnycastIpAddressEntry);
	REEXPORT(DeleteIPAddress);
	REEXPORT(DeleteIpForwardEntry);
	REEXPORT(DeleteIpForwardEntry2);
	REEXPORT(DeleteIpNetEntry);
	REEXPORT(DeleteIpNetEntry2);
	REEXPORT(DeletePersistentTcpPortReservation);
	REEXPORT(DeletePersistentUdpPortReservation);
	REEXPORT(DeleteProxyArpEntry);
	REEXPORT(DeleteUnicastIpAddressEntry);
	REEXPORT(DisableMediaSense);
	REEXPORT(EnableRouter);
	REEXPORT(FlushIpNetTable);
	REEXPORT(FlushIpNetTable2);
	REEXPORT(FlushIpPathTable);
	REEXPORT(FreeMibTable);
	REEXPORT(GetAdapterIndex);
	REEXPORT(GetAdapterOrderMap);
	REEXPORT(GetAdaptersAddresses);
	REEXPORT(GetAdaptersInfo);
	REEXPORT(GetAnycastIpAddressEntry);
	REEXPORT(GetAnycastIpAddressTable);
	REEXPORT(GetBestInterface);
	REEXPORT(GetBestInterfaceEx);
	REEXPORT(GetBestRoute);
	REEXPORT(GetBestRoute2);
	REEXPORT(GetCurrentThreadCompartmentId);
	REEXPORT(GetExtendedTcpTable);
	REEXPORT(GetExtendedUdpTable);
	REEXPORT(GetFriendlyIfIndex);
	REEXPORT(GetIcmpStatistics);
	REEXPORT(GetIcmpStatisticsEx);
	REEXPORT(GetIfEntry);
	REEXPORT(GetIfEntry2);
	REEXPORT(GetIfStackTable);
	REEXPORT(GetIfTable);
	REEXPORT(GetIfTable2);
	REEXPORT(GetIfTable2Ex);
	REEXPORT(GetInterfaceInfo);
	REEXPORT(GetInvertedIfStackTable);
	REEXPORT(GetIpAddrTable);
	REEXPORT(GetIpErrorString);
	REEXPORT(GetIpForwardEntry2);
	REEXPORT(GetIpForwardTable);
	REEXPORT(GetIpForwardTable2);
	REEXPORT(GetIpInterfaceEntry);
	REEXPORT(GetIpInterfaceTable);
	REEXPORT(GetIpNetEntry2);
	REEXPORT(GetIpNetTable);
	REEXPORT(GetIpNetTable2);
	REEXPORT(GetIpNetworkConnectionBandwidthEstimates);
	REEXPORT(GetIpPathEntry);
	REEXPORT(GetIpPathTable);
	REEXPORT(GetIpStatistics);
	REEXPORT(GetIpStatisticsEx);
	REEXPORT(GetMulticastIpAddressEntry);
	REEXPORT(GetMulticastIpAddressTable);
	REEXPORT(GetNetworkInformation);
	REEXPORT(GetNetworkParams);
	REEXPORT(GetNumberOfInterfaces);
	REEXPORT(GetOwnerModuleFromPidAndInfo);
	REEXPORT(GetOwnerModuleFromTcp6Entry);
	REEXPORT(GetOwnerModuleFromTcpEntry);
	REEXPORT(GetOwnerModuleFromUdp6Entry);
	REEXPORT(GetOwnerModuleFromUdpEntry);
	REEXPORT(GetPerAdapterInfo);
	REEXPORT(GetPerTcp6ConnectionEStats);
	REEXPORT(GetPerTcp6ConnectionStats);
	REEXPORT(GetPerTcpConnectionEStats);
	REEXPORT(GetPerTcpConnectionStats);
	REEXPORT(GetRTTAndHopCount);
	REEXPORT(GetSessionCompartmentId);
	REEXPORT(GetTcp6Table);
	REEXPORT(GetTcp6Table2);
	REEXPORT(GetTcpStatistics);
	REEXPORT(GetTcpStatisticsEx);
	REEXPORT(GetTcpTable);
	REEXPORT(GetTcpTable2);
	REEXPORT(GetTeredoPort);
	REEXPORT(GetUdp6Table);
	REEXPORT(GetUdpStatistics);
	REEXPORT(GetUdpStatisticsEx);
	REEXPORT(GetUdpTable);
	REEXPORT(GetUniDirectionalAdapterInfo);
	REEXPORT(GetUnicastIpAddressEntry);
	REEXPORT(GetUnicastIpAddressTable);
	REEXPORT(Icmp6CreateFile);
	REEXPORT(Icmp6ParseReplies);
	REEXPORT(Icmp6SendEcho2);
	REEXPORT(IcmpCloseHandle);
	REEXPORT(IcmpCreateFile);
	REEXPORT(IcmpParseReplies);
	REEXPORT(IcmpSendEcho);
	REEXPORT(IcmpSendEcho2);
	REEXPORT(IcmpSendEcho2Ex);
	REEXPORT(InitializeIpForwardEntry);
	REEXPORT(InitializeIpInterfaceEntry);
	REEXPORT(InitializeUnicastIpAddressEntry);
	REEXPORT(InternalCleanupPersistentStore);
	REEXPORT(InternalCreateAnycastIpAddressEntry);
	REEXPORT(InternalCreateIpForwardEntry);
	REEXPORT(InternalCreateIpForwardEntry2);
	REEXPORT(InternalCreateIpNetEntry);
	REEXPORT(InternalCreateIpNetEntry2);
	REEXPORT(InternalCreateUnicastIpAddressEntry);
	REEXPORT(InternalDeleteAnycastIpAddressEntry);
	REEXPORT(InternalDeleteIpForwardEntry);
	REEXPORT(InternalDeleteIpForwardEntry2);
	REEXPORT(InternalDeleteIpNetEntry);
	REEXPORT(InternalDeleteIpNetEntry2);
	REEXPORT(InternalDeleteUnicastIpAddressEntry);
	REEXPORT(InternalFindInterfaceByAddress);
	REEXPORT(InternalGetAnycastIpAddressEntry);
	REEXPORT(InternalGetAnycastIpAddressTable);
	REEXPORT(InternalGetForwardIpTable2);
	REEXPORT(InternalGetIPPhysicalInterfaceForDestination);
	REEXPORT(InternalGetIfEntry2);
	REEXPORT(InternalGetIfTable);
	REEXPORT(InternalGetIfTable2);
	REEXPORT(InternalGetIpAddrTable);
	REEXPORT(InternalGetIpForwardEntry2);
	REEXPORT(InternalGetIpForwardTable);
	REEXPORT(InternalGetIpInterfaceEntry);
	REEXPORT(InternalGetIpInterfaceTable);
	REEXPORT(InternalGetIpNetEntry2);
	REEXPORT(InternalGetIpNetTable);
	REEXPORT(InternalGetIpNetTable2);
	REEXPORT(InternalGetMulticastIpAddressEntry);
	REEXPORT(InternalGetMulticastIpAddressTable);
	REEXPORT(InternalGetRtcSlotInformation);
	REEXPORT(InternalGetTcp6Table2);
	REEXPORT(InternalGetTcp6TableWithOwnerModule);
	REEXPORT(InternalGetTcp6TableWithOwnerPid);
	REEXPORT(InternalGetTcpTable);
	REEXPORT(InternalGetTcpTable2);
	REEXPORT(InternalGetTcpTableEx);
	REEXPORT(InternalGetTcpTableWithOwnerModule);
	REEXPORT(InternalGetTcpTableWithOwnerPid);
	REEXPORT(InternalGetTunnelPhysicalAdapter);
	REEXPORT(InternalGetUdp6TableWithOwnerModule);
	REEXPORT(InternalGetUdp6TableWithOwnerPid);
	REEXPORT(InternalGetUdpTable);
	REEXPORT(InternalGetUdpTableEx);
	REEXPORT(InternalGetUdpTableWithOwnerModule);
	REEXPORT(InternalGetUdpTableWithOwnerPid);
	REEXPORT(InternalGetUnicastIpAddressEntry);
	REEXPORT(InternalGetUnicastIpAddressTable);
	REEXPORT(InternalIcmpCreateFileEx);
	REEXPORT(InternalSetIfEntry);
	REEXPORT(InternalSetIpForwardEntry);
	REEXPORT(InternalSetIpForwardEntry2);
	REEXPORT(InternalSetIpInterfaceEntry);
	REEXPORT(InternalSetIpNetEntry);
	REEXPORT(InternalSetIpNetEntry2);
	REEXPORT(InternalSetIpStats);
	REEXPORT(InternalSetTcpEntry);
	REEXPORT(InternalSetTeredoPort);
	REEXPORT(InternalSetUnicastIpAddressEntry);
	REEXPORT(IpReleaseAddress);
	REEXPORT(IpRenewAddress);
	REEXPORT(LookupPersistentTcpPortReservation);
	REEXPORT(LookupPersistentUdpPortReservation);
	REEXPORT(NTPTimeToNTFileTime);
	REEXPORT(NTTimeToNTPTime);
	REEXPORT(NhGetGuidFromInterfaceName);
	REEXPORT(NhGetInterfaceDescriptionFromGuid);
	REEXPORT(NhGetInterfaceNameFromDeviceGuid);
	REEXPORT(NhGetInterfaceNameFromGuid);
	REEXPORT(NhpAllocateAndGetInterfaceInfoFromStack);
	REEXPORT(NotifyAddrChange);
	REEXPORT(NotifyCompartmentChange);
	REEXPORT(NotifyIpInterfaceChange);
	REEXPORT(NotifyRouteChange);
	REEXPORT(NotifyRouteChange2);
	REEXPORT(NotifyStableUnicastIpAddressTable);
	REEXPORT(NotifyTeredoPortChange);
	REEXPORT(NotifyUnicastIpAddressChange);
	REEXPORT(OpenCompartment);
	REEXPORT(ParseNetworkString);
	REEXPORT(ResolveIpNetEntry2);
	REEXPORT(ResolveNeighbor);
	REEXPORT(RestoreMediaSense);
	REEXPORT(SendARP);
	REEXPORT(SetAdapterIpAddress);
	REEXPORT(SetCurrentThreadCompartmentId);
	REEXPORT(SetIfEntry);
	REEXPORT(SetIpForwardEntry);
	REEXPORT(SetIpForwardEntry2);
	REEXPORT(SetIpInterfaceEntry);
	REEXPORT(SetIpNetEntry);
	REEXPORT(SetIpNetEntry2);
	REEXPORT(SetIpStatistics);
	REEXPORT(SetIpStatisticsEx);
	REEXPORT(SetIpTTL);
	REEXPORT(SetNetworkInformation);
	REEXPORT(SetPerTcp6ConnectionEStats);
	REEXPORT(SetPerTcp6ConnectionStats);
	REEXPORT(SetPerTcpConnectionEStats);
	REEXPORT(SetPerTcpConnectionStats);
	REEXPORT(SetSessionCompartmentId);
	REEXPORT(SetTcpEntry);
	REEXPORT(SetUnicastIpAddressEntry);
	REEXPORT(UnenableRouter);
	REEXPORT(_PfAddFiltersToInterface);
	REEXPORT(_PfAddGlobalFilterToInterface);
	REEXPORT(_PfBindInterfaceToIPAddress);
	REEXPORT(_PfBindInterfaceToIndex);
	REEXPORT(_PfCreateInterface);
	REEXPORT(_PfDeleteInterface);
	REEXPORT(_PfDeleteLog);
	REEXPORT(_PfGetInterfaceStatistics);
	REEXPORT(_PfMakeLog);
	REEXPORT(_PfRebindFilters);
	REEXPORT(_PfRemoveFilterHandles);
	REEXPORT(_PfRemoveFiltersFromInterface);
	REEXPORT(_PfRemoveGlobalFilterFromInterface);
	REEXPORT(_PfSetLogBuffer);
	REEXPORT(_PfTestPacket);
	REEXPORT(_PfUnBindInterface);
	REEXPORT(do_echo_rep);
	REEXPORT(do_echo_req);
	REEXPORT(if_indextoname);
	REEXPORT(if_nametoindex);
	REEXPORT(register_icmp);
}

MAKE_FUNC(AddIPAddress)
MAKE_FUNC(AllocateAndGetInterfaceInfoFromStack)
MAKE_FUNC(AllocateAndGetIpAddrTableFromStack)
MAKE_FUNC(CancelIPChangeNotify);
MAKE_FUNC(CancelMibChangeNotify2);
MAKE_FUNC(CloseCompartment);
MAKE_FUNC(CloseGetIPPhysicalInterfaceForDestination);
MAKE_FUNC(ConvertCompartmentGuidToId);
MAKE_FUNC(ConvertCompartmentIdToGuid);
MAKE_FUNC(ConvertGuidToStringA);
MAKE_FUNC(ConvertGuidToStringW);
MAKE_FUNC(ConvertInterfaceAliasToLuid);
MAKE_FUNC(ConvertInterfaceGuidToLuid);
MAKE_FUNC(ConvertInterfaceIndexToLuid);
MAKE_FUNC(ConvertInterfaceLuidToAlias);
MAKE_FUNC(ConvertInterfaceLuidToGuid);
MAKE_FUNC(ConvertInterfaceLuidToIndex);
MAKE_FUNC(ConvertInterfaceLuidToNameA);
MAKE_FUNC(ConvertInterfaceLuidToNameW);
MAKE_FUNC(ConvertInterfaceNameToLuidA);
MAKE_FUNC(ConvertInterfaceNameToLuidW);
MAKE_FUNC(ConvertInterfacePhysicalAddressToLuid);
MAKE_FUNC(ConvertIpv4MaskToLength);
MAKE_FUNC(ConvertLengthToIpv4Mask);
MAKE_FUNC(ConvertRemoteInterfaceAliasToLuid);
MAKE_FUNC(ConvertRemoteInterfaceGuidToLuid);
MAKE_FUNC(ConvertRemoteInterfaceIndexToLuid);
MAKE_FUNC(ConvertRemoteInterfaceLuidToAlias);
MAKE_FUNC(ConvertRemoteInterfaceLuidToGuid);
MAKE_FUNC(ConvertRemoteInterfaceLuidToIndex);
MAKE_FUNC(ConvertStringToGuidA);
MAKE_FUNC(ConvertStringToGuidW);
MAKE_FUNC(ConvertStringToInterfacePhysicalAddress);
MAKE_FUNC(CreateAnycastIpAddressEntry);
MAKE_FUNC(CreateIpForwardEntry);
MAKE_FUNC(CreateIpForwardEntry2);
MAKE_FUNC(CreateIpNetEntry);
MAKE_FUNC(CreateIpNetEntry2);
MAKE_FUNC(CreatePersistentTcpPortReservation);
MAKE_FUNC(CreatePersistentUdpPortReservation);
MAKE_FUNC(CreateProxyArpEntry);
MAKE_FUNC(CreateSortedAddressPairs);
MAKE_FUNC(CreateUnicastIpAddressEntry);
MAKE_FUNC(DeleteAnycastIpAddressEntry);
MAKE_FUNC(DeleteIPAddress);
MAKE_FUNC(DeleteIpForwardEntry);
MAKE_FUNC(DeleteIpForwardEntry2);
MAKE_FUNC(DeleteIpNetEntry);
MAKE_FUNC(DeleteIpNetEntry2);
MAKE_FUNC(DeletePersistentTcpPortReservation);
MAKE_FUNC(DeletePersistentUdpPortReservation);
MAKE_FUNC(DeleteProxyArpEntry);
MAKE_FUNC(DeleteUnicastIpAddressEntry);
MAKE_FUNC(DisableMediaSense);
MAKE_FUNC(EnableRouter);
MAKE_FUNC(FlushIpNetTable);
MAKE_FUNC(FlushIpNetTable2);
MAKE_FUNC(FlushIpPathTable);
MAKE_FUNC(FreeMibTable);
MAKE_FUNC(GetAdapterIndex);
MAKE_FUNC(GetAdapterOrderMap);
MAKE_FUNC(GetAdaptersAddresses);
MAKE_FUNC(GetAdaptersInfo);
MAKE_FUNC(GetAnycastIpAddressEntry);
MAKE_FUNC(GetAnycastIpAddressTable);
MAKE_FUNC(GetBestInterface);
MAKE_FUNC(GetBestInterfaceEx);
MAKE_FUNC(GetBestRoute);
MAKE_FUNC(GetBestRoute2);
MAKE_FUNC(GetCurrentThreadCompartmentId);
MAKE_FUNC(GetExtendedTcpTable);
MAKE_FUNC(GetExtendedUdpTable);
MAKE_FUNC(GetFriendlyIfIndex);
MAKE_FUNC(GetIcmpStatistics);
MAKE_FUNC(GetIcmpStatisticsEx);
MAKE_FUNC(GetIfEntry);
MAKE_FUNC(GetIfEntry2);
MAKE_FUNC(GetIfStackTable);
MAKE_FUNC(GetIfTable);
MAKE_FUNC(GetIfTable2);
MAKE_FUNC(GetIfTable2Ex);
MAKE_FUNC(GetInterfaceInfo);
MAKE_FUNC(GetInvertedIfStackTable);
MAKE_FUNC(GetIpAddrTable);
MAKE_FUNC(GetIpErrorString);
MAKE_FUNC(GetIpForwardEntry2);
MAKE_FUNC(GetIpForwardTable);
MAKE_FUNC(GetIpForwardTable2);
MAKE_FUNC(GetIpInterfaceEntry);
MAKE_FUNC(GetIpInterfaceTable);
MAKE_FUNC(GetIpNetEntry2);
MAKE_FUNC(GetIpNetTable);
MAKE_FUNC(GetIpNetTable2);
MAKE_FUNC(GetIpNetworkConnectionBandwidthEstimates);
MAKE_FUNC(GetIpPathEntry);
MAKE_FUNC(GetIpPathTable);
MAKE_FUNC(GetIpStatistics);
MAKE_FUNC(GetIpStatisticsEx);
MAKE_FUNC(GetMulticastIpAddressEntry);
MAKE_FUNC(GetMulticastIpAddressTable);
MAKE_FUNC(GetNetworkInformation);
MAKE_FUNC(GetNetworkParams);
MAKE_FUNC(GetNumberOfInterfaces);
MAKE_FUNC(GetOwnerModuleFromPidAndInfo);
MAKE_FUNC(GetOwnerModuleFromTcp6Entry);
MAKE_FUNC(GetOwnerModuleFromTcpEntry);
MAKE_FUNC(GetOwnerModuleFromUdp6Entry);
MAKE_FUNC(GetOwnerModuleFromUdpEntry);
MAKE_FUNC(GetPerAdapterInfo);
MAKE_FUNC(GetPerTcp6ConnectionEStats);
MAKE_FUNC(GetPerTcp6ConnectionStats);
MAKE_FUNC(GetPerTcpConnectionEStats);
MAKE_FUNC(GetPerTcpConnectionStats);
MAKE_FUNC(GetRTTAndHopCount);
MAKE_FUNC(GetSessionCompartmentId);
MAKE_FUNC(GetTcp6Table);
MAKE_FUNC(GetTcp6Table2);
MAKE_FUNC(GetTcpStatistics);
MAKE_FUNC(GetTcpStatisticsEx);
MAKE_FUNC(GetTcpTable);
MAKE_FUNC(GetTcpTable2);
MAKE_FUNC(GetTeredoPort);
MAKE_FUNC(GetUdp6Table);
MAKE_FUNC(GetUdpStatistics);
MAKE_FUNC(GetUdpStatisticsEx);
MAKE_FUNC(GetUdpTable);
MAKE_FUNC(GetUniDirectionalAdapterInfo);
MAKE_FUNC(GetUnicastIpAddressEntry);
MAKE_FUNC(GetUnicastIpAddressTable);
MAKE_FUNC(Icmp6CreateFile);
MAKE_FUNC(Icmp6ParseReplies);
MAKE_FUNC(Icmp6SendEcho2);
MAKE_FUNC(IcmpCloseHandle);
MAKE_FUNC(IcmpCreateFile);
MAKE_FUNC(IcmpParseReplies);
MAKE_FUNC(IcmpSendEcho);
MAKE_FUNC(IcmpSendEcho2);
MAKE_FUNC(IcmpSendEcho2Ex);
MAKE_FUNC(InitializeIpForwardEntry);
MAKE_FUNC(InitializeIpInterfaceEntry);
MAKE_FUNC(InitializeUnicastIpAddressEntry);
MAKE_FUNC(InternalCleanupPersistentStore);
MAKE_FUNC(InternalCreateAnycastIpAddressEntry);
MAKE_FUNC(InternalCreateIpForwardEntry);
MAKE_FUNC(InternalCreateIpForwardEntry2);
MAKE_FUNC(InternalCreateIpNetEntry);
MAKE_FUNC(InternalCreateIpNetEntry2);
MAKE_FUNC(InternalCreateUnicastIpAddressEntry);
MAKE_FUNC(InternalDeleteAnycastIpAddressEntry);
MAKE_FUNC(InternalDeleteIpForwardEntry);
MAKE_FUNC(InternalDeleteIpForwardEntry2);
MAKE_FUNC(InternalDeleteIpNetEntry);
MAKE_FUNC(InternalDeleteIpNetEntry2);
MAKE_FUNC(InternalDeleteUnicastIpAddressEntry);
MAKE_FUNC(InternalFindInterfaceByAddress);
MAKE_FUNC(InternalGetAnycastIpAddressEntry);
MAKE_FUNC(InternalGetAnycastIpAddressTable);
MAKE_FUNC(InternalGetForwardIpTable2);
MAKE_FUNC(InternalGetIPPhysicalInterfaceForDestination);
MAKE_FUNC(InternalGetIfEntry2);
MAKE_FUNC(InternalGetIfTable);
MAKE_FUNC(InternalGetIfTable2);
MAKE_FUNC(InternalGetIpAddrTable);
MAKE_FUNC(InternalGetIpForwardEntry2);
MAKE_FUNC(InternalGetIpForwardTable);
MAKE_FUNC(InternalGetIpInterfaceEntry);
MAKE_FUNC(InternalGetIpInterfaceTable);
MAKE_FUNC(InternalGetIpNetEntry2);
MAKE_FUNC(InternalGetIpNetTable);
MAKE_FUNC(InternalGetIpNetTable2);
MAKE_FUNC(InternalGetMulticastIpAddressEntry);
MAKE_FUNC(InternalGetMulticastIpAddressTable);
MAKE_FUNC(InternalGetRtcSlotInformation);
MAKE_FUNC(InternalGetTcp6Table2);
MAKE_FUNC(InternalGetTcp6TableWithOwnerModule);
MAKE_FUNC(InternalGetTcp6TableWithOwnerPid);
MAKE_FUNC(InternalGetTcpTable);
MAKE_FUNC(InternalGetTcpTable2);
MAKE_FUNC(InternalGetTcpTableEx);
MAKE_FUNC(InternalGetTcpTableWithOwnerModule);
MAKE_FUNC(InternalGetTcpTableWithOwnerPid);
MAKE_FUNC(InternalGetTunnelPhysicalAdapter);
MAKE_FUNC(InternalGetUdp6TableWithOwnerModule);
MAKE_FUNC(InternalGetUdp6TableWithOwnerPid);
MAKE_FUNC(InternalGetUdpTable);
MAKE_FUNC(InternalGetUdpTableEx);
MAKE_FUNC(InternalGetUdpTableWithOwnerModule);
MAKE_FUNC(InternalGetUdpTableWithOwnerPid);
MAKE_FUNC(InternalGetUnicastIpAddressEntry);
MAKE_FUNC(InternalGetUnicastIpAddressTable);
MAKE_FUNC(InternalIcmpCreateFileEx);
MAKE_FUNC(InternalSetIfEntry);
MAKE_FUNC(InternalSetIpForwardEntry);
MAKE_FUNC(InternalSetIpForwardEntry2);
MAKE_FUNC(InternalSetIpInterfaceEntry);
MAKE_FUNC(InternalSetIpNetEntry);
MAKE_FUNC(InternalSetIpNetEntry2);
MAKE_FUNC(InternalSetIpStats);
MAKE_FUNC(InternalSetTcpEntry);
MAKE_FUNC(InternalSetTeredoPort);
MAKE_FUNC(InternalSetUnicastIpAddressEntry);
MAKE_FUNC(IpReleaseAddress);
MAKE_FUNC(IpRenewAddress);
MAKE_FUNC(LookupPersistentTcpPortReservation);
MAKE_FUNC(LookupPersistentUdpPortReservation);
MAKE_FUNC(NTPTimeToNTFileTime);
MAKE_FUNC(NTTimeToNTPTime);
MAKE_FUNC(NhGetGuidFromInterfaceName);
MAKE_FUNC(NhGetInterfaceDescriptionFromGuid);
MAKE_FUNC(NhGetInterfaceNameFromDeviceGuid);
MAKE_FUNC(NhGetInterfaceNameFromGuid);
MAKE_FUNC(NhpAllocateAndGetInterfaceInfoFromStack);
MAKE_FUNC(NotifyAddrChange);
MAKE_FUNC(NotifyCompartmentChange);
MAKE_FUNC(NotifyIpInterfaceChange);
MAKE_FUNC(NotifyRouteChange);
MAKE_FUNC(NotifyRouteChange2);
MAKE_FUNC(NotifyStableUnicastIpAddressTable);
MAKE_FUNC(NotifyTeredoPortChange);
MAKE_FUNC(NotifyUnicastIpAddressChange);
MAKE_FUNC(OpenCompartment);
MAKE_FUNC(ParseNetworkString);
MAKE_FUNC(ResolveIpNetEntry2);
MAKE_FUNC(ResolveNeighbor);
MAKE_FUNC(RestoreMediaSense);
MAKE_FUNC(SendARP);
MAKE_FUNC(SetAdapterIpAddress);
MAKE_FUNC(SetCurrentThreadCompartmentId);
MAKE_FUNC(SetIfEntry);
MAKE_FUNC(SetIpForwardEntry);
MAKE_FUNC(SetIpForwardEntry2);
MAKE_FUNC(SetIpInterfaceEntry);
MAKE_FUNC(SetIpNetEntry);
MAKE_FUNC(SetIpNetEntry2);
MAKE_FUNC(SetIpStatistics);
MAKE_FUNC(SetIpStatisticsEx);
MAKE_FUNC(SetIpTTL);
MAKE_FUNC(SetNetworkInformation);
MAKE_FUNC(SetPerTcp6ConnectionEStats);
MAKE_FUNC(SetPerTcp6ConnectionStats);
MAKE_FUNC(SetPerTcpConnectionEStats);
MAKE_FUNC(SetPerTcpConnectionStats);
MAKE_FUNC(SetSessionCompartmentId);
MAKE_FUNC(SetTcpEntry);
MAKE_FUNC(SetUnicastIpAddressEntry);
MAKE_FUNC(UnenableRouter);
MAKE_FUNC(_PfAddFiltersToInterface);
MAKE_FUNC(_PfAddGlobalFilterToInterface);
MAKE_FUNC(_PfBindInterfaceToIPAddress);
MAKE_FUNC(_PfBindInterfaceToIndex);
MAKE_FUNC(_PfCreateInterface);
MAKE_FUNC(_PfDeleteInterface);
MAKE_FUNC(_PfDeleteLog);
MAKE_FUNC(_PfGetInterfaceStatistics);
MAKE_FUNC(_PfMakeLog);
MAKE_FUNC(_PfRebindFilters);
MAKE_FUNC(_PfRemoveFilterHandles);
MAKE_FUNC(_PfRemoveFiltersFromInterface);
MAKE_FUNC(_PfRemoveGlobalFilterFromInterface);
MAKE_FUNC(_PfSetLogBuffer);
MAKE_FUNC(_PfTestPacket);
MAKE_FUNC(_PfUnBindInterface);
MAKE_FUNC(do_echo_rep);
MAKE_FUNC(do_echo_req);
MAKE_FUNC(if_indextoname);
MAKE_FUNC(if_nametoindex);
MAKE_FUNC(register_icmp);
