package cn.lentme.learn.gles.activity

import android.os.Bundle
import android.widget.ArrayAdapter
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import cn.lentme.gles.render.GL_HELLO_TRIANGLE2_TYPE
import cn.lentme.gles.render.GL_HELLO_TRIANGLE_TYPE
import cn.lentme.learn.gles.adapter.GLRecyclerAdapter
import cn.lentme.learn.gles.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private lateinit var adapter: GLRecyclerAdapter
    private lateinit var recyclerView: RecyclerView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        initUI();
    }

    private fun initUI() {
        recyclerView = binding.mainRecyclerView
        recyclerView.layoutManager = LinearLayoutManager(this)

        loadType()
    }

    private fun loadType() {
        val mData = ArrayList<Pair<String, Int>>()
        mData.add(Pair("你好, 三角形", GL_HELLO_TRIANGLE_TYPE))
        mData.add(Pair("自定义颜色三角形", GL_HELLO_TRIANGLE2_TYPE))

        adapter = GLRecyclerAdapter(this, mData)
        recyclerView.adapter = adapter
    }
}