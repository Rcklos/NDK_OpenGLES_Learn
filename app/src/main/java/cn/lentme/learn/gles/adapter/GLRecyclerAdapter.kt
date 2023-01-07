package cn.lentme.learn.gles.adapter

import android.content.Context
import android.content.Intent
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import cn.lentme.learn.gles.R
import cn.lentme.learn.gles.activity.GLRenderActivity

class GLRecyclerAdapter(
    private val context: Context,
    private val mData: ArrayList<Pair<String, Int>>
) : RecyclerView.Adapter<GLRecyclerAdapter.GLViewHolder>() {

    inner class GLViewHolder(itemView: View): RecyclerView.ViewHolder(itemView) {
        var mLayout: LinearLayout? = null
        var mTitle: TextView? = null

        init {
            mLayout = itemView.findViewById(R.id.item_main_layout)
            mTitle = itemView.findViewById(R.id.sample_id_tv)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): GLViewHolder {
        val itemView = LayoutInflater.from(context).inflate(R.layout.item_main, parent, false)
        return GLViewHolder(itemView)
    }

    override fun onBindViewHolder(holder: GLViewHolder, position: Int) {
        val entry = mData[position];
        holder.mTitle?.text = entry.first
        holder.mLayout?.let {
            it.isClickable = true
            it.setOnClickListener {
                val intent = Intent(context, GLRenderActivity::class.java)
                intent.putExtra("type", entry.second)
                context.startActivity(intent)
            }
        }
    }

    override fun getItemCount(): Int = mData.size
}