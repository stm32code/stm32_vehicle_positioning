package com.example.SmartGPS

import android.app.DatePickerDialog
import android.app.TimePickerDialog
import android.content.SharedPreferences
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.text.Editable
import android.view.View
import android.widget.TextView
import com.example.SmartGPS.adapter.ListViewAdapter
import com.example.SmartGPS.dao.Heart
import com.example.SmartGPS.dao.HeartDao
import com.example.SmartGPS.databinding.ActivityRecordBinding
import com.example.SmartGPS.utils.MToast
import com.example.SmartGPS.utils.TimeCycle
import com.example.SmartGPS.utils.TimeUtils
import com.gyf.immersionbar.BarHide
import com.gyf.immersionbar.ImmersionBar
import java.util.Calendar

class RecordActivity : AppCompatActivity() {
    private lateinit var binding: ActivityRecordBinding
    private lateinit var sharedPreferences: SharedPreferences // 临时存储
    private lateinit var editor: SharedPreferences.Editor // 修改提交
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityRecordBinding.inflate(layoutInflater)
        sharedPreferences = getSharedPreferences("local", MODE_PRIVATE)
        editor = sharedPreferences.edit()
        setContentView(binding.root)
        initView()
    }

    /**
     * 初始化视图
     */
    private fun initView() {
        //设置共同沉浸式样式
        ImmersionBar.with(this).hideBar(BarHide.FLAG_HIDE_STATUS_BAR).init()
        val text1 = sharedPreferences.getString("Text1", "0")
        val text2 = sharedPreferences.getString("Text2", "0")
        val text3 = sharedPreferences.getString("Text3", "0")
        val text4 = sharedPreferences.getString("Text4", "0")
        // 赋值
        binding.Text1.text = Editable.Factory.getInstance().newEditable(text1)
        binding.Text2.text = Editable.Factory.getInstance().newEditable(text2)
        binding.Text3.text = Editable.Factory.getInstance().newEditable(text3)
        binding.Text4.text = Editable.Factory.getInstance().newEditable(text4)

        eventManager()
    }


    /**
     * 监听事件
     */
    private fun eventManager() {
        binding.backBtn.setOnClickListener {
            finish()
        }

        binding.countBtn.setOnClickListener {
            val text1 = binding.Text1.text.toString()
            val text2 = binding.Text2.text.toString()
            val text3 = binding.Text3.text.toString()
            val text4 = binding.Text4.text.toString()
            if (text1.isEmpty()) {
                MToast.mToast(this, "text1不能为空")
            }
            else if (text2.isEmpty()) {
                MToast.mToast(this, "text2不能为空")
            }
            else if (text3.isEmpty() && text4.isEmpty()) {
                MToast.mToast(this, "text3不能为空")
            }
            else if (text4.isEmpty()) {
                MToast.mToast(this, "text4不能为空")
            }
            else {
                editor.putString("Text1", binding.Text1.text.toString())
                editor.putString("Text2", binding.Text2.text.toString())
                editor.putString("Text3", binding.Text3.text.toString())
                editor.putString("Text4", binding.Text4.text.toString())
                editor.commit()
                MToast.mToast(this, "计算中")
                binding.price1Text.text =
                    (binding.Text1.text.toString().toFloat()/binding.Text2.text.toString().toFloat()
                            / ( binding.Text3.text.toString().toFloat() - binding.Text4.text.toString().toFloat()) * 100 ).toString()
                binding.price2Text.text = (binding.Text1.text.toString().toFloat()
                        /( binding.Text3.text.toString().toFloat() - binding.Text4.text.toString().toFloat())).toString()

            }
        }

    }


}