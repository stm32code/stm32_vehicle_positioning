package com.example.SmartGPS.adapter

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.LinearLayout
import android.widget.TextView
import androidx.core.content.ContextCompat
import com.example.SmartGPS.R
import com.example.SmartGPS.dao.Heart

class ListViewAdapter(var context: Context, var list: MutableList<Heart>) : BaseAdapter() {

    override fun getCount(): Int {
        return list.size
    }

    override fun getItem(p0: Int): Any {
        return list[p0]
    }

    override fun getItemId(p0: Int): Long {
        return p0.toLong()
    }

    override fun getView(p0: Int, p1: View?, p2: ViewGroup?): View? {
        var myHold = MyHold()
        var view = p1
        if (view == null) {
            view = LayoutInflater.from(context).inflate(R.layout.list_view, null)
            view.tag = myHold
        } else {
            myHold = view.tag as MyHold
        }
        initView(view, myHold)
        myHold.layout.background = if (list[p0].isWaring == 1) {
            ContextCompat.getDrawable(context, R.color.red75cency)
        } else {
            ContextCompat.getDrawable(context, R.color.white)
        }
        myHold.heartText.text = list[p0].heart.toString()
        myHold.heartDateTime.text = list[p0].dataTime.toString()
        return view
    }

    private fun initView(view: View?, myHold: MyHold) {
        myHold.layout = view!!.findViewById(R.id.listviewLayout)
        myHold.heartText = view.findViewById(R.id.heartText)
        myHold.heartDateTime = view.findViewById(R.id.heartDateTime)
    }

    class MyHold {
        lateinit var layout: LinearLayout
        lateinit var heartText: TextView
        lateinit var heartDateTime: TextView
    }
}