package com.rekoj134.learnopengleswithc.live_wallpaper

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent

internal class BootBroadCast : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        val service = Intent(context, VideoLiveWallpaperService::class.java)
        context.startService(service)
    }
}